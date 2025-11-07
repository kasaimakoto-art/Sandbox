#include "../include/pixel_buffer.hpp"

namespace kaf::domain::graphics2d{
    PixelBuffer::PixelBuffer(const size_t size, const Pixel& pixel) {
        if(size_ > 0 || pixels_ != nullptr)
            return;
    }

    bool setPixels(PixelBuffer& target, const std::unique_ptr<Pixel[]>&& pixelArray, const size_t size, const size_t startPos){

    }

    bool setPixels(PixelBuffer& target, const std::vector<Pixel>& pixelList, const size_t startPos) {
    }
}