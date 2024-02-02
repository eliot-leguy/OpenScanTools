#ifndef GUI_DATA_HD_H
#define GUI_DATA_HD_H

#include "gui/GuiData/IGuiData.h"
#include "pointCloudEngine/RenderingTypes.h"
#include "io/ImageTypes.h"

#include <cstdint>
#include <filesystem>

#include <glm/glm.hpp>


#include "models/OpenScanToolsModelEssentials.h"

class CameraNode;

class GuiDataPrepareHDImage : public IGuiData
{
public:
    GuiDataPrepareHDImage(bool showFrame, double frameRatio, SafePtr<CameraNode> viewport);
    ~GuiDataPrepareHDImage() {};
    virtual guiDType getType() override;

public:
    bool m_showFrame;
    double m_frameRatio;
    SafePtr<CameraNode> m_viewport;
};


class GuiDataGenerateHDImage : public IGuiData
{
public:
    GuiDataGenerateHDImage(glm::ivec2 imageSize, int multisampling, ImageFormat format, SafePtr<CameraNode> camera, const std::filesystem::path& filepath, const ImageHDMetadata& metadata, bool showProgressBar, uint32_t hdimagetilesize);
    ~GuiDataGenerateHDImage() {};
    virtual guiDType getType() override;

public:
    glm::ivec2 m_imageSize;
    int m_multisampling;
    ImageFormat m_imageFormat;
    SafePtr<CameraNode> m_camera;
    // TODO - Use a viewpoint ID
    std::filesystem::path m_filepath;
    ImageHDMetadata m_metadata;
    bool m_showProgressBar;
    uint32_t m_hdimagetilesize;
};


class GuiDataCallImage : public IGuiData
{
public:
    GuiDataCallImage(bool callhdimage, std::filesystem::path filepath);
    ~GuiDataCallImage() {};
    virtual guiDType getType() override;
public:
    bool m_callHDImage = true;
    std::filesystem::path m_filepath;
};


#endif