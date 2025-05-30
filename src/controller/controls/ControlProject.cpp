#include "controller/controls/ControlProject.h"
#include "controller/controls/ControlApplication.h"
#include "controller/Controller.h"
#include "controller/ControllerContext.h"
#include "controller/functionSystem/FunctionManager.h"
#include "controller/IControlListener.h"
#include "controller/messages/ConvertionMessage.h"
#include "controller/messages/FilesMessage.h"
#include "controller/messages/ImportMessage.h"

#include "gui/GuiData/GuiDataGeneralProject.h"
#include "gui/GuiData/GuiDataAuthor.h"
#include "gui/GuiData/GuiDataRendering.h"
#include "gui/GuiData/GuiDataList.h"
#include "gui/GuiData/GuiDataMessages.h"
#include "gui/GuiData/GuiDataTemplate.h"
#include "gui/GuiData/GuiDataIO.h"
#include "gui/Texts.hpp"
#include "gui/texts/ContextTexts.hpp"

#include "io/FileUtils.h"
#include "io/SaveLoadSystem.h"

#include "models/graph/CameraNode.h"
#include "models/graph/GraphManager.h"

#include "pointCloudEngine/PCE_core.h"

#include "utils/FilesAndFoldersDefinitions.h"
#include "utils/Config.h"
#include "utils/Logger.h"
#include "utils/system.h"

#include <iostream>
#include <filesystem>

// control::project::
#define Yes 0x00004000
#define No 0x00010000
#define Cancel 0x00400000
#define MAX_RECENT_PROJECTS_SIZE 10

namespace control::project
{
    /*
    ** Create
    */

    Create::Create(const ProjectInfos& info, const std::filesystem::path& path, const std::filesystem::path& templatePath)
        : m_projectInfo(info)
        , m_folderPath(path)
        , m_templatePath(templatePath)
        , m_languageTemplate(LanguageType::Nothing)
    {
    }

    Create::Create(const ProjectInfos& info, const std::filesystem::path& path, LanguageType languageTemplate)
        : m_projectInfo(info)
        , m_folderPath(path)
        , m_templatePath("")
        , m_languageTemplate(languageTemplate)
    {
    }

    Create::~Create()
    {
    }

    void Create::doFunction(Controller& controller)
    {
        ControllerContext& context = controller.getContext();
        context.initProjectInfo(m_folderPath, m_projectInfo);

        if (!std::filesystem::exists(m_templatePath))
        {
            controller.getGraphManager().createHierarchyMasterCluster();
            controller.saveCurrentProject(SafePtr<CameraNode>());

            SaveLoadSystem::ErrorCode slsError;
            LanguageType lang = m_languageTemplate;
            if (lang == LanguageType::Nothing)
                lang = Config::getLanguage();

            SaveLoadSystem::ExportTemplates(sma::GenerateDefaultTemplates(lang), slsError, context.cgetProjectInternalInfo().getTemplatesFolderPath() / File_Templates);
            SaveLoadSystem::ExportLists(generateDefaultLists(lang), context.cgetProjectInternalInfo().getTemplatesFolderPath() / File_Lists);
        }
        else
        {
            SaveLoadSystem::loadArboFile(controller, m_templatePath);
            controller.saveCurrentProject(SafePtr<CameraNode>());
            std::error_code error;
            for (const std::filesystem::path& p : std::filesystem::directory_iterator(m_templatePath))
                if (!std::filesystem::is_directory(p) && p.extension() != ".tlc")
                    std::filesystem::copy_file(p, context.cgetProjectInternalInfo().getTemplatesFolderPath() / p.filename(), std::filesystem::copy_options::overwrite_existing, error);
        }

        controller.getGraphManager().cleanProjectObjects();

        controller.getControlListener()->notifyUIControl(new control::project::Load(context.cgetProjectInternalInfo().getProjectFilePath()));

        // TODO
        controller.scantra_notify_project_created();
        CONTROLLOG << "control::project::create[end] " << m_folderPath / m_projectInfo.m_projectName << LOGENDL;
    }

    bool Create::canUndo() const
    {
        return (false);
    }

    void Create::undoFunction(Controller& controller)
    {
    }

    ControlType Create::getType() const
    {
        return (ControlType::createProject);
    }

    /*
    ** ControlDropLoad
    */


    DropLoad::DropLoad(const std::filesystem::path& loadPath)
        : m_loadPath(loadPath)
    {
        std::cout << "/////////////////////" << std::endl;
    }

    DropLoad::~DropLoad()
    {
    }

    void DropLoad::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::DropLoad" << LOGENDL;
        controller.getFunctionManager().launchFunction(controller, ContextType::saveCloseLoadProject);
        FilesMessage message({ m_loadPath });
        controller.getFunctionManager().feedMessage(controller, &message);
    }

    bool DropLoad::canUndo() const
    {
        return false;
    }

    void DropLoad::undoFunction(Controller& controller)
    {
    }

    ControlType DropLoad::getType() const
    {
        return (ControlType::dropLoadProject);
    }

    /*
    ** ControlLoad
    */

    Load::Load(std::filesystem::path _path)
        : m_loadPath(_path)
    {
    }

    Load::~Load()
    {
    }


    void Load::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::Load[begin]" << LOGENDL;
        // Load the new project
        std::string errorMsg;

        SaveLoadSystem::ImportJsonProject(m_loadPath, controller, errorMsg);

        ControllerContext& context = controller.getContext();
        if (!context.isProjectLoaded())
        {
            controller.updateInfo(new GuiDataWarning(QString(TEXT_PROJECT_ERROR_LOADING).arg(errorMsg.c_str())));
            controller.updateInfo(new GuiDataProjectLoaded(false, L"Error: unable to load project!"));
            return;
        }

        // Reset du projet
        controller.getContext().setIsCurrentProjectSaved(true);
        controller.updateInfo(new GuiDataProjectLoaded(true, context.cgetProjectInfo().m_projectName));

        controller.updateInfo(new GuiDataProjectPath(m_loadPath.parent_path()));

        controller.updateInfo(new GuiDataRenderBackgroundColor(SafePtr<CameraNode>(), controller.getContext().getActiveBackgroundColor()));
        controller.updateInfo(new GuiDataTolerances(context.cgetProjectInfo().m_beamBendingTolerance, context.cgetProjectInfo().m_columnTiltTolerance));

        controller.updateInfo(new GuiDataSendTemplateList(controller.getContext().getTemplates()));

        controller.updateInfo(new GuiDataCameraInfo(controller.getGraphManager().getCameraNode()));

        controller.updateInfo(new GuiDataGlobalColorPickerValue(controller.getContext().getActiveColor()));

        controller.updateInfo(new GuiDataSendListsList(controller.getContext().getUserLists()));

        std::unordered_set<SafePtr<StandardList>> pipeStandards = controller.getContext().getStandards(StandardType::Pipe);
        controller.updateInfo(new GuiDataSendListsStandards(StandardType::Pipe, pipeStandards));
        if (!pipeStandards.empty())
            controller.getContext().setCurrentStandard(*pipeStandards.begin(), StandardType::Pipe);

        std::unordered_set<SafePtr<StandardList>> sphereStandards = controller.getContext().getStandards(StandardType::Sphere);
        controller.updateInfo(new GuiDataSendListsStandards(StandardType::Sphere, sphereStandards));
        if (!sphereStandards.empty())
            controller.getContext().setCurrentStandard(*sphereStandards.begin(), StandardType::Sphere);
        controller.updateInfo(new GuiDataSendAuthorsList(controller.getContext().getProjectAuthors(), true));
        controller.updateInfo(new GuiDataTmpMessage());

        controller.updateInfo(new GuiDataProjectProperties(controller.getContext(), controller.getGraphManager(), false));

        /*Note (Aurélien) : Temporary (or not), set default clipping parameters*/
        const ProjectInfos& info(controller.getContext().cgetProjectInfo());
        controller.updateInfo(new GuiDataDefaultClipParams(info.m_defaultMinClipDistance, info.m_defaultMaxClipDistance, info.m_defaultClipMode));
        controller.updateInfo(new GuiDataDefaultRampParams(info.m_defaultMinRampDistance, info.m_defaultMaxRampDistance, info.m_defaultRampSteps));

        std::vector<std::pair<std::filesystem::path, time_t>> toSaveRecentProjects = controller.getContext().getRecentProjects();

        time_t timeNow = time(&timeNow);
        std::filesystem::path newRecentProjectPath = context.cgetProjectInternalInfo().getProjectFilePath();
        bool pathAlreadyContained = false;

        for (auto& pair : toSaveRecentProjects)
        {
            if (pair.first == newRecentProjectPath)
            {
                pair = { newRecentProjectPath, timeNow };
                pathAlreadyContained = true;
                break;
            }
        }
        if (pathAlreadyContained == false)
            toSaveRecentProjects.push_back({ newRecentProjectPath, timeNow });

        if (toSaveRecentProjects.size() > MAX_RECENT_PROJECTS_SIZE) {

            sort(toSaveRecentProjects.begin(), toSaveRecentProjects.end(), [](std::pair< std::filesystem::path, time_t> a, std::pair< std::filesystem::path, time_t> b) {return a.second > b.second; });

            for (int i = 0; i < (toSaveRecentProjects.size() - MAX_RECENT_PROJECTS_SIZE); i++)
                toSaveRecentProjects.pop_back();
        }

        controller.getControlListener()->notifyUIControl(new control::application::SetRecentProjects(toSaveRecentProjects, true));

        controller.changeSelection({});
        // TODO
        controller.scantra_notify_project_opened();
        CONTROLLOG << "control::project::Load[end] " << m_loadPath << LOGENDL;
    }

    bool Load::canUndo() const
    {
        return (false);
    }

    void Load::undoFunction(Controller& controller)
    {
    }

    ControlType Load::getType() const
    {
        return (ControlType::loadProject);
    }

    /*
    ** StartSave
    */

    StartSave::StartSave()
    {
    }

    StartSave::~StartSave()
    {
    }

    void StartSave::doFunction(Controller& controller)
    {
        controller.getFunctionManager().launchFunction(controller, ContextType::saveProject);
    }

    bool StartSave::canUndo() const
    {
        return (false);
    }

    void StartSave::undoFunction(Controller& controller)
    {
    }

    ControlType StartSave::getType() const
    {
        return (ControlType::saveProjectContext);
    }

    /*
    ** Save
    */

    Save::Save(const SafePtr<CameraNode>& camera)
        : m_camera(camera)
    {
    }

    Save::~Save()
    {
    }

    void Save::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::Save" << LOGENDL;
        controller.saveCurrentProject(m_camera);
        controller.updateInfo(new GuiDataTmpMessage(TEXT_SAVE_COMPLETE, 3000));
    }

    bool Save::canUndo() const
    {
        return (false);
    }

    void Save::undoFunction(Controller& controller)
    {
    }

    ControlType Save::getType() const
    {
        return (ControlType::saveProject);
    }

    /*
    ** Close
    */

    Close::Close()
    {
    }

    Close::~Close()
    {
    }

    void Close::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::Close[begin]" << LOGENDL;

        ControllerContext& context = controller.getContext();

        controller.updateInfo(new GuiDataSplashScreenStart(TEXT_PROJECT_CLOSING, GuiDataSplashScreenStart::SplashScreenType::Display));

        // Close all project info in the Gui
        controller.getGraphManager().cleanProjectObjects();
        controller.updateInfo(new GuiDataUndoRedoAble(false, false));
        if (context.isProjectLoaded())
            controller.updateInfo(new GuiDataProjectLoaded(false, context.cgetProjectInfo().m_projectName));
        controller.getContext().setIsCurrentProjectSaved(true);
        // Reset the project in the Controller

        controller.resetHistoric();

        context.cleanProjectInfo();

        uint8_t counter(0);
        while (tlScanLeftToFree() == true && counter < 0xFF)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter++;
        }
        controller.updateInfo(new GuiDataSplashScreenEnd(GuiDataSplashScreenEnd::SplashScreenType::Display));

        CONTROLLOG << "control::project::Close[end]" << LOGENDL;
    }

    bool Close::canUndo() const
    {
        return (false);
    }

    void Close::undoFunction(Controller& controller)
    {
    }

    ControlType Close::getType() const
    {
        return (ControlType::closeProject);
    }

    /*
    ** SaveCreate
    */

    SaveCreate::SaveCreate()
        : init_path_("")
        , init_name_(L"")
        , init_company_(L"")
    {
    }

    SaveCreate::SaveCreate(std::filesystem::path init_path, std::wstring init_name, std::wstring init_company)
        : init_path_(init_path)
        , init_name_(init_name)
        , init_company_(init_company)
    {
    }

    SaveCreate::~SaveCreate()
    {
    }

    void SaveCreate::doFunction(Controller& controller)
    {
        controller.getFunctionManager().launchFunction(controller, ContextType::saveCloseCreateProject);

        if (init_path_ == "")
            init_path_ = controller.getContext().getProjectsPath();

        GuiDataNewProject* gui_data = new GuiDataNewProject();
        gui_data->default_folder_ = init_path_;
        gui_data->default_name_ = init_name_;
        gui_data->default_company_ = init_company_;
        gui_data->templates_ = Utils::System::getFolderFromDirectory(Utils::System::getOSTProgramDataTemplatePath());

        controller.updateInfo(gui_data);
    }

    bool SaveCreate::canUndo() const
    {
        return false;
    }

    void SaveCreate::undoFunction(Controller& controller)
    {
    }

    ControlType SaveCreate::getType() const
    {
        return ControlType::saveCreateProject;
    }

    /*
    ** SaveClose
    */

    SaveClose::SaveClose()
    {
    }

    SaveClose::~SaveClose()
    {
    }

    void SaveClose::doFunction(Controller& controller)
    {
        controller.getFunctionManager().launchFunction(controller, ContextType::saveCloseProject);
    }

    bool SaveClose::canUndo() const
    {
        return false;
    }

    void SaveClose::undoFunction(Controller& controller)
    {
    }

    ControlType SaveClose::getType() const
    {
        return ControlType::saveCloseProject;
    }

    /*
    ** SaveCloseLoad
    */

    SaveCloseLoad::SaveCloseLoad(std::filesystem::path path)
    {
        m_projectToLoad = path;
    }

    SaveCloseLoad::~SaveCloseLoad()
    {
    }

    void SaveCloseLoad::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::SaveCloseLoad" << LOGENDL;
        controller.getFunctionManager().launchFunction(controller, ContextType::saveCloseLoadProject);
        if (m_projectToLoad.empty())
            controller.updateInfo(new GuiDataOpenProject(controller.getContext().getProjectsPath()));
        else
        {
            FilesMessage message({ m_projectToLoad });
            controller.getFunctionManager().feedMessage(controller, &message);
        }
    }

    bool SaveCloseLoad::canUndo() const
    {
        return false;
    }

    void SaveCloseLoad::undoFunction(Controller& controller)
    {
    }

    ControlType SaveCloseLoad::getType() const
    {
        return ControlType::saveCloseLoadProject;
    }

    /*
    ** SaveQuit
    */

    SaveQuit::SaveQuit()
    {
    }

    SaveQuit::~SaveQuit()
    {
    }

    void SaveQuit::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::application::SaveQuit" << LOGENDL;

        controller.getFunctionManager().launchFunction(controller, ContextType::saveQuitProject);
    }

    bool SaveQuit::canUndo() const
    {
        return (false);
    }

    void SaveQuit::undoFunction(Controller& controller)
    {
    }

    ControlType SaveQuit::getType() const
    {
        return (ControlType::saveQuitProject);
    }

    /*
    ** Edit
    */

    Edit::Edit(const ProjectInfos& infos)
    {
        newInfos = infos;
    }

    Edit::~Edit()
    {
    }

    void Edit::doFunction(Controller& controller)
    {
        CONTROLLOG << "control::project::Edit" << LOGENDL;
        oldInfos = controller.getContext().getProjectInfo();

        controller.getContext().setProjectInfo(newInfos);
        controller.updateInfo(new GuiDataProjectProperties(controller.getContext(), controller.getGraphManager()));
    }

    bool Edit::canUndo() const
    {
        return (true);
    }

    void Edit::undoFunction(Controller& controller)
    {
        controller.getContext().setProjectInfo(oldInfos);
        controller.updateInfo(new GuiDataProjectProperties(controller.getContext(), controller.getGraphManager()));
    }

    ControlType Edit::getType() const
    {
        return (ControlType::editProject);
    }

    /*
    ** FunctionImportScan
    */
    FunctionImportScan::FunctionImportScan()
    {
    }

    FunctionImportScan::~FunctionImportScan()
    {
    }

    void FunctionImportScan::doFunction(Controller& controller)
    {
        controller.updateInfo(new GuiDataImportScans());
    }

    bool FunctionImportScan::canUndo() const
    {
        return false;
    }

    void FunctionImportScan::undoFunction(Controller& controller)
    {
    }

    ControlType FunctionImportScan::getType() const
    {
        return (ControlType::functionImportScanProject);
    }

    /*
    ** ImportScan
    */

    ImportScan::ImportScan(const Import::ScanInfo& importInfo)
    {
        m_importInfo = importInfo;
    }

    ImportScan::~ImportScan()
    {
    }

    void ImportScan::doFunction(Controller& controller)
    {

        if (!controller.getContext().isProjectLoaded())
            return;

        std::vector<std::filesystem::path> toConvert;
        std::vector<std::filesystem::path> toImport;

        for (const std::filesystem::path file_path : m_importInfo.paths)
        {
            FileType file_type = FileUtils::getType(file_path);

            switch (file_type)
            {
            case FileType::FARO_PROJ:
            case FileType::FARO_LS:
            case FileType::E57:
            case FileType::RCS:
            case FileType::RCP:
                toConvert.push_back(file_path);
                break;

            case FileType::PTS:
            {
                if (m_importInfo.mapAsciiInfo.find(file_path) != m_importInfo.mapAsciiInfo.end())
                    toConvert.push_back(file_path);
            }
            break;

            case FileType::TLS:
                toImport.push_back(file_path);
                break;
            default:
                break;
            }
        }

        if (!toConvert.empty())
        {
            controller.getFunctionManager().launchFunction(controller, ContextType::scanConversion);
            m_importInfo.paths = toConvert;
            ImportScanMessage message(m_importInfo);
            controller.getFunctionManager().feedMessage(controller, &message);
        }
        if (!toImport.empty())
        {
            controller.getFunctionManager().launchFunction(controller, ContextType::scanImport);
            m_importInfo.paths = toImport;
            ImportScanMessage message(m_importInfo);
            controller.getFunctionManager().feedMessage(controller, &message);
        }

        CONTROLLOG << "control::project::ImportScan" << LOGENDL;
    }

    bool ImportScan::canUndo() const
    {
        return (false);
    }

    void ImportScan::undoFunction(Controller& controller)
    {
    }

    ControlType ImportScan::getType() const
    {
        return (ControlType::importScanProject);
    }

    /*
    ** ConvertScan
    */

    ConvertScan::ConvertScan(const ConvertProperties& prop)
        : m_prop(prop)
    {
    }

    ConvertScan::~ConvertScan()
    {
    }

    void ConvertScan::doFunction(Controller& controller)
    {
        ConvertionMessage message(m_prop);
        controller.getFunctionManager().feedMessage(controller, &message);
    }

    bool ConvertScan::canUndo() const
    {
        return (false);
    }

    void ConvertScan::undoFunction(Controller& controller)
    {
    }

    ControlType ConvertScan::getType() const
    {
        return (ControlType::showConvertionOptions);
    }

    /*
    ** ShowProperties
    */

    ShowProperties::ShowProperties()
    {
    }

    ShowProperties::~ShowProperties()
    {
    }

    void ShowProperties::doFunction(Controller& controller)
    {
        controller.updateInfo(new GuiDataProjectProperties(controller.getContext(), controller.getGraphManager()));
        CONTROLLOG << "control::project::ShowProperties" << LOGENDL;
    }

    bool ShowProperties::canUndo() const
    {
        return (false);
    }

    void ShowProperties::undoFunction(Controller& controller)
    {
    }

    ControlType ShowProperties::getType() const
    {
        return (ControlType::showPropertiesProject);
    }
}