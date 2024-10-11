#include "controller/controls/ControlApplication.h"
#include "utils/Logger.h"
#include "controller/Controller.h"
#include "models/graph/GraphManager.h"
#include "models/graph/ManipulatorNode.h"

#include "gui/GuiData/GuiDataGeneralProject.h"
#include "gui/GuiData/GuiDataRendering.h"
#include "gui/GuiData/GuiData3dObjects.h"
#include "controller/ControllerContext.h"
#include "controller/controls/ControlProject.h"
#include "controller/functionSystem/FunctionManager.h"
#include "controller/messages/UndoRedoMessages.h"
#include "io/SaveLoadSystem.h"
#include "gui/GuiData/GuiDataMessages.h"
#include "gui/GuiData/GuiDataIO.h"
#include "gui/Translator.h"
#include "gui/Texts.hpp"
#include "utils/Config.h"
#include "utils/Logger.h"


// control::application::
namespace control
{
	namespace application
	{
        /*
        ** Quit
        */

        Quit::Quit()
        {
        }

        Quit::~Quit()
        { }

        void Quit::doFunction(Controller& controller)
        {
            CONTROLLOG << "control::application::Quit" << LOGENDL;

            controller.updateInfo(new GuiDataQuit());
        }

        bool Quit::canUndo() const
        {
            return (false);
        }

        void Quit::undoFunction(Controller& controller)
        {
        }

        ControlType Quit::getType() const
        {
            return (ControlType::quitApplication);
        }

		/*
		** Undo
		*/

		Undo::Undo()
		{
		}

		Undo::~Undo()
		{
		}

		void Undo::doFunction(Controller& controller)
		{

			CONTROLLOG << "control::application::Undo" << LOGENDL;
			controller.undoLastAction();
		}

		bool Undo::canUndo() const
		{
			return (false);
		}

		void Undo::undoFunction(Controller& controller)
		{
		}

		ControlType Undo::getType() const
		{
			return (ControlType::undoApplication);
		}

		/*
		** Redo
		*/

		Redo::Redo()
		{
		}

		Redo::~Redo()
		{
		}

		void Redo::doFunction(Controller& controller)
		{
			//NOTE (Aur�lien) POC Undo/Redo context
			CONTROLLOG << "control::application::Redo" << LOGENDL;
			controller.redoLastAction();
		}

		bool Redo::canUndo() const
		{
			return (false);
		}

		void Redo::undoFunction(Controller& controller)
		{
		}

		ControlType Redo::getType() const
		{
			return (ControlType::redoApplication);
		}

		namespace author
		{

			/*
			** SaveAndQuitAuthors
			*/

			SaveAndQuitAuthors::SaveAndQuitAuthors()
			{
			}

			SaveAndQuitAuthors::~SaveAndQuitAuthors()
			{
			}

			void SaveAndQuitAuthors::doFunction(Controller& controller)
			{
				SaveLoadSystem::ErrorCode errorMsg;
				SaveLoadSystem::saveAuthors(controller.getContext().getLocalAuthors(), errorMsg);
				assert(errorMsg == SaveLoadSystem::ErrorCode::Success);

				controller.updateInfo(new GuiDataSendAuthorsList(controller.getContext().getLocalAuthors(), -1));
				if (errorMsg!= SaveLoadSystem::ErrorCode::Success)
					controller.updateInfo(new GuiDataWarning(TEXT_WRITE_FAILED_PERMISSION));

				ReadPtr<Author> rAuth = controller.getContext().getActiveAuthor().cget();
				if (!rAuth || !rAuth->getId().isValid())
					controller.updateInfo(new GuiDataWarning(TEXT_WARNING_AUTHOR_WRONG_CLOSE));
				else
				{
					controller.updateInfo(new GuiDataCloseAuthorsList());
					controller.updateInfo(new GuiDataAuthorNameSelection(rAuth->getName()));
				}
				CONTROLLOG << "control::application::SaveAndQuitAuthors" << LOGENDL;
			}

			bool SaveAndQuitAuthors::canUndo() const
			{
				return (false);
			}

			void SaveAndQuitAuthors::undoFunction(Controller& controller)
			{
			}

			ControlType SaveAndQuitAuthors::getType() const
			{
				return (ControlType::saveAuthors);
			}

			/*
			** SelectAuthor
			*/

			SelectAuthor::SelectAuthor(SafePtr<Author> author)
			{
				m_authorSelected = author;
			}

			SelectAuthor::~SelectAuthor()
			{
			}

			void SelectAuthor::doFunction(Controller& controller)
			{
				controller.getContext().setActiveAuthor(m_authorSelected);
				ReadPtr<Author> rAuth = m_authorSelected.cget();
				if(rAuth)
					CONTROLLOG << "control::application::SelectAuthor " << rAuth->getId() << LOGENDL;
				else
					CONTROLLOG << "control::application::SelectAuthor cannot select" << LOGENDL;
			}

			bool SelectAuthor::canUndo() const
			{
				return (false);
			}

			void SelectAuthor::undoFunction(Controller& controller)
			{
			}

			ControlType SelectAuthor::getType() const
			{
				return (ControlType::selectAuthor);
			}

			/*
			** CreateNewAuthor
			*/

			CreateNewAuthor::CreateNewAuthor(const std::wstring& authorName)
			{
				m_authorNameToCreate = authorName;
			}

			CreateNewAuthor::~CreateNewAuthor()
			{
			}

			void CreateNewAuthor::doFunction(Controller& controller)
			{
				CONTROLLOG << "control::application::CreateNewAuthor" << LOGENDL;
				bool nameAlreadyExist = false;
				for (const SafePtr<Author>& auth : controller.getContext().getLocalAuthors())
				{
					ReadPtr<Author> rAuth = auth.cget();
					if (rAuth && rAuth->getName() == m_authorNameToCreate)
					{
						nameAlreadyExist = true;
						break;
					}
				}
				if (nameAlreadyExist)
				{
					controller.updateInfo(new GuiDataWarning(TEXT_WARNING_AUTHOR_CREATE));
					return;
				}

				SafePtr<Author> newAuth = make_safe<Author>(m_authorNameToCreate);
				controller.getContext().addLocalAuthors({ newAuth });

				controller.updateInfo(new GuiDataSendAuthorsList(controller.getContext().getLocalAuthors(), -1));
			}

			bool CreateNewAuthor::canUndo() const
			{
				return (false);
			}

			void CreateNewAuthor::undoFunction(Controller& controller)
			{
			}

			ControlType CreateNewAuthor::getType() const
			{
				return (ControlType::createAuthor);
			}

			/*
			** DeleteAuthor
			*/

			DeleteAuthor::DeleteAuthor(SafePtr<Author> author)
			{
				m_authorToDelete = author;
			}

			DeleteAuthor::~DeleteAuthor()
			{
			}

			void DeleteAuthor::doFunction(Controller& controller)
			{
				controller.getContext().remLocalAuthors({ m_authorToDelete });
				controller.updateInfo(new GuiDataSendAuthorsList(controller.getContext().getLocalAuthors(), -1));
				CONTROLLOG << "control::application::DeleteAuthor" << LOGENDL;
			}

			bool DeleteAuthor::canUndo() const
			{
				return (false);
			}

			void DeleteAuthor::undoFunction(Controller& controller)
			{
			}

			ControlType DeleteAuthor::getType() const
			{
				return (ControlType::deleteAuthor);
			}

			/*
			** SendAuthorList
			*/

			SendAuthorList::SendAuthorList()
			{
			}

			SendAuthorList::~SendAuthorList()
			{
			}

			void SendAuthorList::doFunction(Controller& controller)
			{
				std::unordered_set<SafePtr<Author>> authors(controller.getContext().getLocalAuthors());

				int index(0);
				for (const SafePtr<Author>& auth : authors)
				{
					if (auth == controller.getContext().getActiveAuthor())
						break;
					index++;
				}
				if (index == authors.size())
					index = 0;

				controller.updateInfo(new GuiDataSendAuthorsList(authors, index));
				CONTROLLOG << "control::application::SendAuthorList" << LOGENDL;
			}

			bool SendAuthorList::canUndo() const
			{
				return (false);
			}

			void SendAuthorList::undoFunction(Controller& controller)
			{
			}

			ControlType SendAuthorList::getType() const
			{
				return (ControlType::sendAuthorList);
			}
		}

		/*
		** SetLangage
		*/

		SetLangage::SetLangage(const LangageType& type)
			: m_type(type)
		{}

		SetLangage::~SetLangage()
		{}

		void SetLangage::doFunction(Controller& controller)
		{
			if (Config::setLangage(m_type))
				controller.updateInfo(new GuiDataInfo(TEXT_SETTINGS_NEED_RESTART_LANGUAGE, true));
			else
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetLangage" << LOGENDL;
		}

		bool SetLangage::canUndo() const
		{
			return (false);
		}

		void SetLangage::undoFunction(Controller& controller)
		{}

		ControlType SetLangage::getType() const
		{
			return (ControlType::setLangage);
		}

		/*
		** SetValueSettingsDisplay
		*/


		SetValueSettingsDisplay::SetValueSettingsDisplay(const UnitUsage& valueParameters, const bool & save) : m_parameters(valueParameters), m_save(save)
		{}

		SetValueSettingsDisplay::~SetValueSettingsDisplay()
		{}

		void SetValueSettingsDisplay::doFunction(Controller & controller)
		{
			controller.getContext().m_unitUsage = m_parameters;
			controller.updateInfo(new GuiDataRenderUnitUsage(m_parameters, SafePtr<CameraNode>()));
			if (m_save && !Config::setUnitUsageConfiguration(m_parameters))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetValueSettingsDisplay" << LOGENDL;
		}
		bool SetValueSettingsDisplay::canUndo() const
		{
			return false;
		}
		void SetValueSettingsDisplay::undoFunction(Controller & controller)
		{
		}
		ControlType SetValueSettingsDisplay::getType() const
		{
			return ControlType::setValueSettingsDisplay;
		}

		/*
		** SetProjectsFolder
		*/

		SetProjectsFolder::SetProjectsFolder(const std::filesystem::path& path, const bool& save)
			: m_path(path)
			, m_save(save)
		{}

		SetProjectsFolder::~SetProjectsFolder()
		{}

		void SetProjectsFolder::doFunction(Controller& controller)
		{
			controller.getContext().setProjectsPath(m_path);
			if (m_save && !Config::setProjectsPath(m_path))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetProjectsFolder" << LOGENDL;
		}

		bool SetProjectsFolder::canUndo() const
		{
			return (false);
		}

		void SetProjectsFolder::undoFunction(Controller& controller)
		{}

		ControlType SetProjectsFolder::getType() const
		{
			return (ControlType::setProjectsFolder);
		}

		/*
		** SetTemporaryFolder
		*/

		SetTemporaryFolder::SetTemporaryFolder(const std::filesystem::path& path, const bool& save)
			: m_path(path)
			, m_save(save)
		{}

		SetTemporaryFolder::~SetTemporaryFolder()
		{}

		void SetTemporaryFolder::doFunction(Controller& controller)
		{
			controller.getContext().setTemporaryPath(m_path);
			if (m_save && !Config::setTemporaryPath(m_path))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			controller.updateInfo(new GuiDataTemporaryPath(m_path));
			CONTROLLOG << "control::application::SetTemporaryFolder" << LOGENDL;
		}

		bool SetTemporaryFolder::canUndo() const
		{
			return (false);
		}

		void SetTemporaryFolder::undoFunction(Controller& controller)
		{}

		ControlType SetTemporaryFolder::getType() const
		{
			return (ControlType::setTemporaryFolder);
		}

		/*
		** SetUserColor
		*/

		SetUserColor::SetUserColor(const Color32& color, const uint32_t& position, const bool& set, const bool& save)
			: m_color(color)
			, m_position(position)
			, m_save(save)
			, m_set(set)
		{}

		SetUserColor::~SetUserColor()
		{}

		void SetUserColor::doFunction(Controller& controller)
		{
			controller.getContext().setUserBackgroundColor(m_color, m_position);
			if (m_save && !Config::setUserColor(m_color))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			if(m_set)
				controller.updateInfo(new GuiDataRenderBackgroundColor(SafePtr<CameraNode>(), m_color));
			CONTROLLOG << "control::application::SetTemporaryFolder" << LOGENDL;
		}

		bool SetUserColor::canUndo() const
		{
			return (false);
		}
		void SetUserColor::undoFunction(Controller& controller)
		{}

		ControlType SetUserColor::getType() const
		{
			return  (ControlType::setUserColor);
		}

        /*
        ** SetDecimationOptions
        */

        SetDecimationOptions::SetDecimationOptions(const DecimationOptions& options, const bool& set, const bool& save)
            : m_options(options)
            , m_save(save)
            , m_set(set)
        {}

        SetDecimationOptions::~SetDecimationOptions()
        {}

        void SetDecimationOptions::doFunction(Controller& controller)
        {
            controller.getContext().setDecimationOptions(m_options);
            if (m_save && !Config::setDecimationOptions(m_options))
                controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
            if (m_set)
                controller.updateInfo(new GuiDataRenderDecimationOptions(m_options));
        }

        bool SetDecimationOptions::canUndo() const
        {
            return (false);
        }
        void SetDecimationOptions::undoFunction(Controller& controller)
        {}

        ControlType SetDecimationOptions::getType() const
        {
            return  (ControlType::setDecimationOptions);
        }

        /*
        ** SetRenderPointSize
        */

        SetRenderPointSize::SetRenderPointSize(const uint32_t& pointSize, SafePtr<CameraNode> camera)
            : m_camera(camera)
			, m_pointSize(pointSize)
        {}

        SetRenderPointSize::~SetRenderPointSize()
        {}

        void SetRenderPointSize::doFunction(Controller& controller)
        {
            controller.getContext().setRenderPointSize(m_pointSize);
            controller.updateInfo(new GuiDataRenderPointSize(m_pointSize, m_camera));
        }

        bool SetRenderPointSize::canUndo() const
        {
            return (false);
        }
        void SetRenderPointSize::undoFunction(Controller& controller)
        {}

        ControlType SetRenderPointSize::getType() const
        {
            return  (ControlType::setRenderPointSize);
        }


		/* 
		** SetExamineCentering
		*/

		SetExamineOptions::SetExamineOptions(bool isCentering, bool keepOnPan, bool save)
			: m_isCentering(isCentering)
			, m_keepOnPan(keepOnPan)
			, m_save(save)
		{}

		SetExamineOptions::~SetExamineOptions()
		{}

		void SetExamineOptions::doFunction(Controller& controller)
		{
			controller.updateInfo(new GuiDataExamineOptions(m_isCentering, m_keepOnPan));
			if (m_save &&
				(!Config::setCenteringConfiguration(m_isCentering) || !Config::setKeepingExamineConfiguration(m_keepOnPan)))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetExamineOptions" << LOGENDL;
		}

		bool SetExamineOptions::canUndo() const
		{
			return false;
		}

		void SetExamineOptions::undoFunction(Controller & controller)
		{}

		ControlType SetExamineOptions::getType() const
		{
			return ControlType::setExamineOptions;
		}

		/*
		** SetFramelessMode
		*/

		SetFramelessMode::SetFramelessMode(const bool& isFrameless, const bool& save)
			: m_isFrameless(isFrameless)
			, m_save(save)
		{}
			
		SetFramelessMode::~SetFramelessMode()
		{}

		void SetFramelessMode::doFunction(Controller& controller) 
		{
			if (m_save && !Config::setMaximizedFrameless(m_isFrameless))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetFramelessMode" << LOGENDL;
		}

		bool SetFramelessMode::canUndo() const 
		{
			return false;
		}

		void SetFramelessMode::undoFunction(Controller& controller)
		{}

		ControlType SetFramelessMode::getType() const
		{
			return ControlType::setFramelessMode;
		}

		/*
		** SetExamineDisplayMode
		*/

		SetExamineDisplayMode::SetExamineDisplayMode(bool isActive, bool save)
			: m_isActive(isActive)
			, m_save(save)
		{}

		SetExamineDisplayMode::~SetExamineDisplayMode()
		{}

		void SetExamineDisplayMode::doFunction(Controller& controller)
		{
			controller.updateInfo(new GuiDataRenderExamineTarget(m_isActive));
			if (m_save && !Config::setExamineDisplayMode(m_isActive))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetExamineDisplayMode" << LOGENDL;
		}

		bool SetExamineDisplayMode::canUndo() const
		{
			return false;
		}

		void SetExamineDisplayMode::undoFunction(Controller& controller)
		{}

		ControlType SetExamineDisplayMode::getType() const
		{
			return ControlType::setExamineDisplayMode;
		}

		/*
		** SetRecentProjects
		*/

		SetRecentProjects::SetRecentProjects(const std::vector<std::pair<std::filesystem::path, time_t>>& recentProjects, const bool & save)
			: m_save(save)
		{
			for (auto pair : recentProjects)
			{
				try {
					if (std::filesystem::exists(pair.first))
						m_recentProjects.push_back(pair);
				}
				catch (...)
				{
					continue;
				}
			}
		}

		SetRecentProjects::~SetRecentProjects()
		{}

		void SetRecentProjects::doFunction(Controller & controller)
		{
			controller.getContext().setRecentProjects(m_recentProjects);
			if (m_save && !Config::setRecentProjects(m_recentProjects))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetRecentProjects" << LOGENDL;
		}

		bool SetRecentProjects::canUndo() const
		{
			return false;
		}
		void SetRecentProjects::undoFunction(Controller & controller)
		{
		}
		ControlType SetRecentProjects::getType() const
		{
			return ControlType::setRecentProjects;
		}

		/*
		** SetRecentProjects
		*/

		SendRecentProjects::SendRecentProjects()
		{
		}

		SendRecentProjects::~SendRecentProjects()
		{
		}

		void SendRecentProjects::doFunction(Controller & controller)
		{
			std::vector<std::pair<std::filesystem::path, time_t>> toSend = controller.getContext().getRecentProjects();

			sort(toSend.begin(), toSend.end(), [](std::pair<std::filesystem::path, time_t> a, std::pair<std::filesystem::path, time_t> b) {return (a.second > b.second); });

			controller.updateInfo(new GuiDataSendRecentProjects(toSend));
			CONTROLLOG << "control::application::SendRecentProjects" << LOGENDL;
		}

		bool SendRecentProjects::canUndo() const
		{
			return false;
		}

		void SendRecentProjects::undoFunction(Controller & controller)
		{
		}

		ControlType SendRecentProjects::getType() const
		{
			return ControlType::sendRecentProjects;
		}


		/*
		** RenderModeUpdate
		*/

		RenderModeUpdate::RenderModeUpdate(const UiRenderMode& mode, SafePtr<CameraNode> camera)
			: m_mode(mode)
			, m_camera(camera)
		{}

		RenderModeUpdate::~RenderModeUpdate()
		{}

		void RenderModeUpdate::doFunction(Controller& controller)
		{
			controller.updateInfo(new GuiDataRenderColorMode(m_mode, m_camera));
			m_clusterColor = (m_mode == UiRenderMode::Clusters_Color);
			if (m_clusterColor != controller.getContext().getShowClusterColors())
				controller.getContext().setShowClusterColors(m_clusterColor);
		}

		bool RenderModeUpdate::canUndo() const
		{
			return (false);
		}

		void RenderModeUpdate::undoFunction(Controller& controller)
		{}

		ControlType RenderModeUpdate::getType() const
		{
			return ControlType::renderModeUpdate;
		}

		/*
		** SetAutoSaveParameters
		*/

		SetAutoSaveParameters::SetAutoSaveParameters(const bool& activate, const uint8_t& timing, const bool& save)
			: m_isActivate(activate)
			, m_timing(timing)
			, m_save(save)
		{}

		SetAutoSaveParameters::~SetAutoSaveParameters()
		{}

		void SetAutoSaveParameters::doFunction(Controller& controller)
		{
			if (m_isActivate)
			{
				controller.activateAutosave(m_timing);
				CONTROLLOG << "control::application::SetAutoSaveParameters started." << LOGENDL;
			}
			else
			{
				controller.deactivateAutosave();
				CONTROLLOG << "control::application::SetAutoSaveParameters stoped." << LOGENDL;
			}

			if (m_save && !Config::setIsAutoSaveActive(m_isActivate) && !Config::setAutoSaveTiming(m_timing))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetAutoSaveParameters parameters activate: "<< m_isActivate << " ,timing: "<< std::to_string(m_timing) << LOGENDL;
		}

		bool SetAutoSaveParameters::canUndo() const
		{
			return false;
		}

		void SetAutoSaveParameters::undoFunction(Controller& controller)
		{}

		ControlType SetAutoSaveParameters::getType() const
		{
			return ControlType::setIndexationMethod;
		}

		/*
		**	SetIndexationMethod
		*/

		SetIndexationMethod::SetIndexationMethod(const IndexationMethod& indexationMethod, const bool& save)
			: m_indexationMethod(indexationMethod), m_save(save)
		{}

		SetIndexationMethod::~SetIndexationMethod()
		{}

		void SetIndexationMethod::doFunction(Controller& controller)
		{
			controller.getContext().setIndexationMethod(m_indexationMethod);
			if (m_save && !Config::setIndexationMethod(m_indexationMethod))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			CONTROLLOG << "control::application::SetIndexationMethod" << LOGENDL;
		}

		bool SetIndexationMethod::canUndo() const
		{
			return false;
		}

		void SetIndexationMethod::undoFunction(Controller& controller)
		{
		}

		ControlType SetIndexationMethod::getType() const
		{
			return ControlType::setIndexationMethod;
		}

		/*
		**	SetGizmoParameters
		*/
		SetGizmoParameters::SetGizmoParameters(const bool& activate, const glm::vec3& gizmo, const bool& save)
			: m_save(save)
			, m_isActivate(activate)
			, m_gizmo(gizmo)
		{}	 

		SetGizmoParameters::~SetGizmoParameters()
		{}

		void SetGizmoParameters::doFunction(Controller& controller)
		{
			if (m_save && !Config::setGizmoParameters(&m_gizmo.x))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			if (m_isActivate)
				controller.updateInfo(new GuiDataGizmoParameters(m_gizmo));
			CONTROLLOG << "control::application::SetGizmoParameters" << LOGENDL;
		}

		bool SetGizmoParameters::canUndo() const
		{
			return false;
		}

		void SetGizmoParameters::undoFunction(Controller& controller)
		{}
			
		ControlType SetGizmoParameters::getType() const
		{
			return ControlType::setGizmoParameters;
		}
		

		/*
		**	SetManipulatorSize
		*/

		SetManipulatorSize::SetManipulatorSize(float sizeFactor, bool save)
			: m_sizeFactor(sizeFactor)
			, m_save(save)
		{}

		SetManipulatorSize::~SetManipulatorSize()
		{
		}

		void SetManipulatorSize::doFunction(Controller& controller)
		{
			if (m_save && !Config::setManipulatorSize(m_sizeFactor))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			controller.updateInfo(new GuiDataManipulatorSize(m_sizeFactor));
			CONTROLLOG << "control::application::SetManipulatorSize" << LOGENDL;
		}

		bool SetManipulatorSize::canUndo() const
		{
			return false;
		}

		void SetManipulatorSize::undoFunction(Controller& controller)
		{}

		ControlType SetManipulatorSize::getType() const
		{
			return ControlType::setManipulatorSize;
		}

		/*
		** SetNavigationParameters
		*/

		SetNavigationParameters::SetNavigationParameters(const NavigationParameters& navParam, const bool& save)
			: m_navParam(navParam)
			, m_save(save)
		{}

		SetNavigationParameters::~SetNavigationParameters()
		{}

		void SetNavigationParameters::doFunction(Controller& controller)
		{
			if (m_save && !Config::setNavigationParameters(m_navParam))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			controller.updateInfo(new GuiDataRenderNavigationParameters(m_navParam));
			CONTROLLOG << "control::application::SetManipulatorSize" << LOGENDL;
		}

		bool SetNavigationParameters::canUndo() const
		{
			return false;
		}

		void SetNavigationParameters::undoFunction(Controller& controller)
		{}

		ControlType SetNavigationParameters::getType() const
		{
			return ControlType::setNavigationParameters;
		}

		/*
		** SetPerspectiveZBounds
		*/

		SetPerspectiveZBounds::SetPerspectiveZBounds(const PerspectiveZBounds& zBounds, const bool& save)
			: m_zBounds(zBounds)
			, m_save(save)
		{}

		SetPerspectiveZBounds::~SetPerspectiveZBounds()
		{}

		void SetPerspectiveZBounds::doFunction(Controller& controller)
		{
			if (m_save && !Config::setPerspectiveZBounds(m_zBounds))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			controller.updateInfo(new GuiDataRenderPerspectiveZBounds(m_zBounds, SafePtr<CameraNode>()));
			CONTROLLOG << "control::application::SetManipulatorSize" << LOGENDL;
		}

		bool SetPerspectiveZBounds::canUndo() const
		{
			return false;
		}

		void SetPerspectiveZBounds::undoFunction(Controller& controller)
		{}

		ControlType SetPerspectiveZBounds::getType() const
		{
			return ControlType::setPerspectiveZBounds;
		}

		/*
		** SetOrthographicZBounds
		*/

		SetOrthographicZBounds::SetOrthographicZBounds(const OrthographicZBounds& zBounds, const bool& save)
			: m_zBounds(zBounds)
			, m_save(save)
		{}

		SetOrthographicZBounds::~SetOrthographicZBounds()
		{}

		void SetOrthographicZBounds::doFunction(Controller& controller)
		{
			if (m_save && !Config::setOrthographicZBounds(m_zBounds))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));
			controller.updateInfo(new GuiDataRenderOrthographicZBounds(m_zBounds, SafePtr<CameraNode>()));
			CONTROLLOG << "control::application::SetManipulatorSize" << LOGENDL;
		}

		bool SetOrthographicZBounds::canUndo() const
		{
			return false;
		}

		void SetOrthographicZBounds::undoFunction(Controller& controller)
		{}

		ControlType SetOrthographicZBounds::getType() const
		{
			return ControlType::setPerspectiveZBounds;
		}

		/*
		** UnlockScanManipulation
		*/

		UnlockScanManipulation::UnlockScanManipulation(bool unlock, bool save)
			: m_unlock(unlock)
			, m_save(save)
		{}

		UnlockScanManipulation::~UnlockScanManipulation()
		{}

		void UnlockScanManipulation::doFunction(Controller& controller)
		{
			if (m_save && !Config::setUnlockScanManipulation(m_unlock))
				controller.updateInfo(new GuiDataWarning(TEXT_SETTINGS_FAILED_TO_SAVE));

			ManipulatorNode::setScanManipulable(m_unlock);

			CONTROLLOG << "control::application::SetManipulatorSize" << LOGENDL;
		}

		ControlType UnlockScanManipulation::getType() const
		{
			return ControlType::unlockScanManipulation;
		}

		/*
		** SetOrthoGridParameters
		*/

		SetOrthoGridParameters::SetOrthoGridParameters(bool active, const Color32& color, float step, uint32_t linewidth)
			: m_active(active)
			, m_color(color)
			, m_step(step)
			, m_linewidth(linewidth)
		{}

		SetOrthoGridParameters::~SetOrthoGridParameters()
		{}

		void SetOrthoGridParameters::doFunction(Controller& controller)
		{
			const SafePtr<CameraNode>& camera = controller.getGraphManager().getCameraNode();
			WritePtr<CameraNode> wCamera = camera.get();
			if (!wCamera)
				return;

			wCamera->m_orthoGridActive = m_active;
			wCamera->m_orthoGridColor = m_color;
			wCamera->m_orthoGridStep = m_step;
			wCamera->m_orthoGridLineWidth = m_linewidth;
		}

		ControlType SetOrthoGridParameters::getType() const
		{
			return ControlType::setOrthoGridParams;
		}
	}
}