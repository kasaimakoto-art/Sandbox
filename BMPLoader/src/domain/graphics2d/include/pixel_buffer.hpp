#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#include <memory>
#include <vector>
#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    struct PixelBuffer{
        PixelBuffer(const size_t size, const Pixel& pixel = Pixel(1.0f, 1.0f, 1.0f));
        bool isValid() const { return size_ > 0 && pixels_ != nullptr; }
        size_t size_{};
        std::unique_ptr<Pixel[]> pixels_{};
    };

    bool setPixels(PixelBuffer& target, const std::unique_ptr<Pixel[]>& pixelArray, const size_t size, const size_t startPos = 0);
    bool setPixels(PixelBuffer& target, const std::vector<Pixel>& pixelList, const size_t startPos = 0);
    bool setPixel(PixelBuffer& target, const Pixel& pixel, const size_t pos);
    bool getPixel(const PixelBuffer& buffer, Pixel& pixel, const size_t pos);
}

#endif