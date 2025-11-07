#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#include <memory>
#include <vector>
#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    struct PixelBuffer{
        PixelBuffer(const size_t size, const Pixel& pixel = Pixel(1.0f, 1.0f, 1.0f));
        size_t size_{};
        std::unique_ptr<Pixel[]> pixels_{};
    };

    bool setPixels(PixelBuffer& target, const std::unique_ptr<Pixel[]> pixelArray, const size_t size, const size_t startPos = 0);
    bool setPixels(PixelBuffer& target, const std::vector<Pixel> pixelList, const size_t startPos = 0);
}

#endif