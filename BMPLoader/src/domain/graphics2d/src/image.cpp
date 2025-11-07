#include "../include/image.hpp"
#include "../include/pixel.hpp"
#include "../include/pixel_buffer.hpp"

namespace kaf::domain::graphics2d{

    bool Image::isValid() const {
        if(!pixelBuffer_){
            return false;
        }
        if(!pixelBuffer_->isValid()){
            return false;
        }
        auto expectedSize = mul_size(width_, height_);
        if(!expectedSize.has_value()){
            return false;
        }
        if(pixelBuffer_->size_ < expectedSize.value()){
            return false;
        }
        return true;
    }

    bool getPixel(const Image& image, Pixel& pixel, size_t width, size_t height){
        if(!image.pixelBuffer_){
            return false;
        }
        if(!image.isValid()){
            return false;
        }
        if(image.width_ < width){
            return false;
        }
        if(image.height_ < height){
            return false;
        }
        return getPixel(*image.pixelBuffer_, pixel, height * image.width_ + width);
    }

    bool setPixel(Image& image, const Pixel& pixel, size_t width, size_t height){
        if(!image.pixelBuffer_){
            return false;
        }
        if(!image.isValid()){
            return false;
        }
        if(image.width_ < width){
            return false;
        }
        if(image.height_ < height){
            return false;
        }
        return setPixel(*image.pixelBuffer_, pixel, height * image.width_ + width);
    }

    std::unique_ptr<Image> createImage(const std::unique_ptr<PixelBuffer>&& buffer, const size_t width, const size_t height) {
        auto expectedSize = mul_size(width, height);
        if(!expectedSize.has_value()){
            return false;
        }
        if(buffer->size_ < expectedSize.value()){
            return false;
        }
        return std::make_unique<Image>(std::move(buffer), height, width);
    }

}