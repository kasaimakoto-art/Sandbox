/**
 * @file pixel_buffer.cpp
 * @brief PixelBuffer と関連関数の実装。
 */
#include "../include/pixel_buffer.hpp"
#include "../include/pixel.hpp"

#include <algorithm>

namespace kaf::domain::graphics2d{
    PixelBuffer::PixelBuffer(const size_t size, const Pixel& pixel) {
        if(size_ > 0 || pixels_ != nullptr) { pixels_ = nullptr; }
        std::unique_ptr<Pixel[]> pixelArray = std::make_unique<Pixel[]>(size);
        if(!pixelArray) { return; }
        size_ = size;
        std::fill(pixelArray.get(), pixelArray.get() + size, pixel);
        pixels_ = std::move(pixelArray);
    }

    bool setPixels(PixelBuffer& target, const std::unique_ptr<Pixel[]>& pixelArray, const size_t size, const size_t startPos){
        if(target.isValid() && pixelArray && size > 0 && startPos + size <= target.size_){
            std::copy(pixelArray.get(), pixelArray.get() + size, target.pixels_.get() + startPos);
            return true;
        }
        return false;
    }

    bool setPixels(PixelBuffer& target, const std::vector<Pixel>& pixelList, const size_t startPos) {
        if(target.isValid() && !pixelList.empty() && startPos + pixelList.size() <= target.size_){
            std::copy(pixelList.begin(), pixelList.end(), target.pixels_.get() + startPos);
            return true;
        }
        return false;
    }
    bool setPixel(PixelBuffer& target, const Pixel& pixel, const size_t pos){
        if(target.isValid() && pos < target.size_){
            target.pixels_[pos] = pixel;
            return true;
        }
        return false;
    }
    bool getPixel(const PixelBuffer& buffer, Pixel& pixel, const size_t pos){
        if(buffer.isValid() && pos < buffer.size_){
            pixel = buffer.pixels_[pos];
            return true;
        }
        return false;
    }
}
