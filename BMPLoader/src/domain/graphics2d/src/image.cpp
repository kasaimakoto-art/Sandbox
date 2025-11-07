#include "../include/image.hpp"
#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    bool getPixel(const Image& image, Pixel& pixel, size_t width, size_t height){
        if(!image.pixels_){
            return false;
        }
        if(image.width_ < width){
            return false;
        }
        if(image.height_ < height){
            return false;
        }
        pixel = image.pixels_[height * image.width_ + width];
        return true;
    }

    bool setPixel(Image& image, const Pixel& pixel, size_t width, size_t height){
        if(!image.pixels_){
            return false;
        }
        if(image.width_ < width){
            return false;
        }
        if(image.height_ < height){
            return false;
        }
        Pixel& target = image.pixels_[height * image.width_ + width];
        target = pixel;
        return true;
    }
    
    // std::unique_ptr<Image> createImage(std::unique_ptr<Pixel[]> data, size_t bufferSize, size_t width, size_t height) {
    //     if(bufferSize < mul_size(width, height)) return nullptr;
    //     return std::make_unique<Image>(std::move(data), height, width);
    // }

    std::unique_ptr<Image> createImage(const std::unique_ptr<Pixel[]>& data, const size_t bufferSize, const size_t width, const size_t height) {
        if(bufferSize < mul_size(width, height)) return nullptr;
        std::unique_ptr<Pixel[]> dataBuffer = std::make_unique<Pixel[]>(bufferSize);
        memcpy(data.get(), dataBuffer.get(), bufferSize);
        return std::make_unique<Image>(std::move(dataBuffer), height, width);
    }

    std::unique_ptr<Image> createImage(std::vector<Pixel>& data, const size_t bufferSize, const size_t width, const size_t height) {
        if(bufferSize < mul_size(width, height)) return nullptr;
        std::unique_ptr<Pixel[]> dataBuffer = std::make_unique<Pixel[]>(bufferSize);
        std::copy_n(data.data(), bufferSize, dataBuffer.get());
        return std::make_unique<Image>(std::move(dataBuffer), height, width);
    }

}