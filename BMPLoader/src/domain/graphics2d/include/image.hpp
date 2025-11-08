#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <optional>
#include <vector>
#include <memory>


namespace kaf::domain::graphics2d{
struct Pixel;
struct PixelBuffer;
    struct Image{
        explicit Image() = default;
        explicit Image(std::unique_ptr<PixelBuffer>&& buffer, size_t width, size_t height): 
            pixelBuffer_(std::move(buffer)), width_(width), height_(height){}
        bool isValid() const;
        size_t width_{};
        size_t height_{};
        std::unique_ptr<PixelBuffer> pixelBuffer_ = nullptr;
    };

    inline std::optional<size_t> mul_size(size_t width, size_t height) {
        if(width == 0 || height == 0) return 0;
        if(width > std::numeric_limits<size_t>::max() / height) return std::nullopt;
        return width * height;
    }

    bool getPixel(const Image& image, Pixel& pixel, unsigned int width, unsigned int height);
    bool setPixel(Image& image, const Pixel& pixel, unsigned int width, unsigned int height);
    std::unique_ptr<Image> createImage(std::unique_ptr<PixelBuffer>&& buffer, size_t width, size_t height);
    std::unique_ptr<Image> copyImage(const PixelBuffer& buffer, size_t width, size_t height);
}

#endif