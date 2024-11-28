#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_

#include "vulkan/ImageTransferEvent.h"
#include "io/ImageTypes.h"

#include <filesystem>

class ImageWriter
{
public:
    ImageWriter();
    ~ImageWriter();

    bool saveScreenshot(const std::filesystem::path& filepath, ImageFormat format, ImageTransferEvent transfer, uint32_t width, uint32_t height);

    bool startCapture(ImageFormat format, uint32_t width, uint32_t height);
    void transferImageTile(ImageTransferEvent transfer, uint32_t dstOffsetW, uint32_t dstOffsetH);
    bool save(const std::filesystem::path& file_path, ImageHDMetadata metadata);

private:
    bool saveMetadata(const std::filesystem::path& path, ImageHDMetadata metadata);

    bool saveImage(const std::filesystem::path& filepath);

    bool allocateBuffer();

private:
    char* image_buffer_ = nullptr;
    size_t buffer_size_ = 0;

    uint32_t width_ = 0;
    uint32_t height_ = 0;
    ImageFormat format_ = ImageFormat::MAX_ENUM;
    uint32_t byte_per_pixel_ = 4;

    ImageHDMetadata metadata_;
};

#endif //! IMAGE_MANAGER_H_