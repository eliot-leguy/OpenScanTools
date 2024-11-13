#ifndef CONTEXT_EXPORT_PC_H_
#define CONTEXT_EXPORT_PC_H_

#include "controller/functionSystem/AContext.h"
#include "io/exports/ExportParameters.hpp"
#include "models/data/Clipping/ClippingGeometry.h"
#include "models/project/ProjectInfos.h"
#include "models/pointCloud/PointCloudInstance.h"
#include "models/graph/APointCloudNode.h"
#include "utils/safe_ptr.h"

#include <vector>
#include <glm/glm.hpp>
#include <unordered_set>

enum FileType;
enum PrecisionType;
class IScanFileWriter;
class CSVWriter;

class GraphManager;
class CameraNode;

class ContextExportPC : public AContext
{
public:
    ContextExportPC(const ContextId& id);
    ~ContextExportPC();
    ContextState start(Controller& controller) override;
    ContextState launch(Controller& controller) override;
    ContextState feedMessage(IMessage* message, Controller& controller) override;
    bool canAutoRelaunch() const;

    ContextType getType() const override;

protected:
    // One export to bind them all
    struct CopyTask {
        std::filesystem::path src_path;
        std::filesystem::path dst_path;
        tls::Transformation dst_transfo;
    };
    struct ExportTask {
        std::wstring file_name;
        tls::ScanHeader header;
        TransformationModule transfo;
        std::vector<tls::PointCloudInstance> input_pcs; // if empty, take all the point clouds
        ClippingAssembly clippings;
    };
    bool processExport(Controller& controller, CSVWriter* csv_writer);
    void copyTls(Controller& controller, CopyTask task);
    void prepareExportTasks(Controller& controller, std::vector<ExportTask>& export_tasks);
    void prepareCopyTasks(Controller& controller, std::vector<CopyTask>& copy_tasks);

    void logStart(Controller& controller, size_t total_steps);
    void logProgress(Controller& controller);
    void logEnd(Controller& controller, bool success);

    bool processGridExport(Controller& controller);

    // helper functions
    void addOriginCube(IScanFileWriter* fileWriter, tls::PointFormat pointFormat, CSVWriter& csvWriter);
    bool ensureFileWriter(Controller& controller, std::unique_ptr<IScanFileWriter>& scanFileWriter, std::wstring name, CSVWriter* csvWriter);
    bool prepareOutputDirectory(Controller& controller, const std::filesystem::path& folderPath);
    std::vector<tls::PointCloudInstance> getPointCloudInstances(Controller& controller);

    TransformationModule getBestTransformation(const ClippingAssembly& clipping_assembly, const std::vector<tls::PointCloudInstance>& pc_instances);
    // Should be static functions for BoundingBox
    static BoundingBoxD extractBBox(const IClippingGeometry& clippingGeom);
    // !! static //
    tls::PointFormat getCommonFormat(const std::vector<tls::PointCloudInstance>& pcInfos);

protected:
    int m_neededMessageCount;
    bool m_forSubProject;
    SafePtr<CameraNode> m_cameraNode;
    ClippingExportParameters m_parameters;

private:
    std::unordered_set<SafePtr<APointCloudNode>> m_selectedPcs;
    ContextId m_saveContext;
    xg::Guid m_viewportId;
    bool m_exportScans;
    bool m_exportPCOs;
    bool m_useClips;
    bool m_useGrids;
    glm::dvec3 m_scanTranslationToAdd = glm::dvec3(0.);

    std::chrono::steady_clock::time_point process_time_;
    size_t total_steps_;
    size_t current_step_;
};

class ContextExportSubProject : public ContextExportPC
{
public:
    ContextExportSubProject(const ContextId& id);
    ~ContextExportSubProject();
    ContextState start(Controller& controller) override;
    ContextState launch(Controller& controller) override;
    ContextState feedMessage(IMessage* message, Controller& controller) override;
    ContextState abort(Controller& controller) override;
    bool canAutoRelaunch() const;

    void exportObjects(Controller& controller) const;

    ContextType getType() const override;

private:
    ProjectInfos m_subProjectInfo;
    ProjectInternalInfo m_subProjectInternal;
    ObjectStatusFilter m_objectFilterType;
};

#endif // !CONTEXT_GRID_CLIPPING_H_