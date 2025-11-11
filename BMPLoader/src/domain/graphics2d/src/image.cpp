/**
 * @file image.cpp
 * @brief Image と関連ユーティリティの実装。
 */
#include "../include/image.hpp"
#include "../include/pixel.hpp"
#include "../include/pixel_buffer.hpp"

#include <optional>
#include <limits>
#include <memory>
#include <algorithm>

namespace kaf::domain::graphics2d{
    Image::Image()
    : pixelBuffer_(nullptr), width_(0), height_(0){}

    Image::Image(std::unique_ptr<PixelBuffer>&& buffer, size_t width, size_t height)
        : pixelBuffer_(std::move(buffer)), width_(width), height_(height){}

    Image::Image(const size_t width, const size_t height, const Pixel& pixel){
        std::optional<size_t> size = domain::graphics2d::mul_size(width, height);
        if(!size.has_value()) return;
        std::unique_ptr<domain::graphics2d::PixelBuffer> tmpBuffer = std::make_unique<PixelBuffer>(size.value(), pixel);
        if(tmpBuffer == nullptr){
            return;
        }
        setPixelBuffer(std::move(tmpBuffer));
        setHeight(height);
        setWidth(width);
    }

    Image::~Image(){
        width_ = 0;
        height_ = 0;
        pixelBuffer_ = nullptr;
    }
    Image::Image(const Image& other){
        if(other.getPixelBuffer() != nullptr){
            auto otherBuffer = other.getPixelBuffer();
            std::unique_ptr<PixelBuffer> tmpBuffer = std::make_unique<PixelBuffer>(otherBuffer->size_);
            if(tmpBuffer == nullptr) return;
            bool result = domain::graphics2d::setPixels(*tmpBuffer, otherBuffer->pixels_, otherBuffer->size_, 0);
            if(!result){
                tmpBuffer = nullptr;
                return;
            }
            setPixelBuffer(std::move(tmpBuffer));
        } else {
            setPixelBuffer(nullptr);
        }
        setHeight(other.getHeight());
        setWidth(other.getWidth());
    }
    Image& Image::operator=(const Image& other){
        if(this == &other){
            return *this;
        }
        if(other.getPixelBuffer() != nullptr){
            auto otherBuffer = other.getPixelBuffer();
            std::unique_ptr<domain::graphics2d::PixelBuffer> tmpBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(otherBuffer->size_);
            if(tmpBuffer == nullptr) return *this;
            bool result = domain::graphics2d::setPixels(*tmpBuffer, otherBuffer->pixels_, otherBuffer->size_, 0);
            if(!result){
                tmpBuffer = nullptr;
                return *this;
            }
            setPixelBuffer(std::move(tmpBuffer));
        } else {
            setPixelBuffer(nullptr);
        }
        setHeight(other.getHeight());
        setWidth(other.getWidth());
        return *this;
    }
    Image::Image(Image&& other){
        setHeight(other.getHeight());
        setWidth(other.getWidth());
        setPixelBuffer(other.passPixelBuffer());
    }
    Image& Image::operator=(Image&& other){
        if(this == &other){
            return *this;
        }
        setHeight(other.getHeight());
        setWidth(other.getWidth());
        setPixelBuffer(other.passPixelBuffer());
        return *this;
    }

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

    Pixel* Image::getPixel(size_t width, size_t height)const {
        if(!getPixelBuffer()){
            return nullptr;
        }
        if(!isValid()){
            return nullptr;
        }
        if(getWidth() < width){
            return nullptr;
        }
        if(getHeight() < height){
            return nullptr;
        }
        return &(getPixelBuffer()->pixels_[height * width_ + width]);
    }

    bool Image::setPixel(size_t width, size_t height, const Pixel& pixel ){
        if(!getPixelBuffer()){
            return false;
        }
        if(!isValid()){
            return false;
        }
        if(getWidth() < width){
            return false;
        }
        if(getHeight() < height){
            return false;
        }
        getPixelBuffer()->pixels_[height * width_ + width] = pixel;
        return true;
    }

    std::unique_ptr<Image> createImage(std::unique_ptr<PixelBuffer>&& buffer, const size_t width, const size_t height) {
        auto expectedSize = mul_size(width, height);
        if(!expectedSize.has_value()){
            return nullptr;
        }
        if(buffer->size_ < expectedSize.value()){
            return nullptr;
        }
        return std::make_unique<Image>(std::move(buffer), width, height);
    }

    std::unique_ptr<Image> copyImage(const PixelBuffer& buffer, size_t width, size_t height) {
        auto expectedSize = mul_size(width, height);
        if(!expectedSize.has_value()){
            return nullptr;
        }
        if(buffer.size_ != expectedSize.value()){
            return nullptr;
        }
        auto newBuffer = std::make_unique<PixelBuffer>(expectedSize.value());
        if(!newBuffer || !newBuffer->isValid()){
            return nullptr;
        }
        std::copy(buffer.pixels_.get(), buffer.pixels_.get() + expectedSize.value(), newBuffer->pixels_.get());
        return std::make_unique<Image>(std::move(newBuffer), width, height);
    }

    std::unique_ptr<Image> createImage(const std::vector<Pixel>& pixels, size_t width, size_t height) {
        auto expectedSize = mul_size(width, height);
        if(!expectedSize.has_value()){
            return nullptr;
        }
        if(pixels.size() != expectedSize.value()){
            return nullptr;
        }
        auto newBuffer = std::make_unique<PixelBuffer>(expectedSize.value());
        if(!newBuffer || !newBuffer->isValid()){
            return nullptr;
        }
        std::copy(pixels.begin(), pixels.end(), newBuffer->pixels_.get());
        return std::make_unique<Image>(std::move(newBuffer), width, height);
    }

}
