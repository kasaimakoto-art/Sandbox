/**
 * @file bmp.cpp
 * @brief BMP 読み書きクラスの実装（進行中）。
 */
#include "../include/bmp.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>

#include "../../../domain/graphics2d/include/image.hpp"
#include "../../../domain/graphics2d/include/pixel.hpp"
#include "../../../domain/graphics2d/include/pixel_buffer.hpp"

namespace kaf::infra::codecs{

    BMP::BMP(): ::kaf::domain::graphics2d::Image(){
        // Default constructor
    }

    BMP::BMP(size_t width, size_t height): ::kaf::domain::graphics2d::Image(){
        if(pixelBuffer_ != nullptr){
            pixelBuffer_ = nullptr;
        }
        std::optional<size_t> size = domain::graphics2d::mul_size(width, height);
        if(!size.has_value()) return;
        pixelBuffer_ = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(pixelBuffer_ == nullptr) return;
        width_ = width;
        height_ = height;
    }

    bool BMP::setImage(const domain::graphics2d::PixelBuffer& pixelBuffer, size_t width, size_t height){
        std::optional<size_t> size = domain::graphics2d::mul_size(width, height);
        if(!size.has_value()) return false;
        if(!pixelBuffer.isValid()) return false;
        if(pixelBuffer.size_ != size.value()) return false;
        pixelBuffer_ = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(pixelBuffer_ == nullptr) return false;
        bool result = domain::graphics2d::setPixels(*pixelBuffer_, pixelBuffer.pixels_, size.value(), 0);
        if(!result){
            pixelBuffer_ = nullptr;
            return false;
        }
        width_ =  width;
        height_ = height;
        return true;
    }

    bool BMP::loadImage(const std::string& inputFilePath){
        if(pixelBuffer_ != nullptr){ pixelBuffer_ =nullptr; }
        std::filesystem::path filePath(inputFilePath);
        if(!std::filesystem::exists(filePath)) {return false;}
        std::ifstream inputFile(filePath.c_str(), std::ios::binary);
        if(!inputFile.is_open()){
            return false;
        }
        if(!readBitmapFileHeader(inputFile)){
            inputFile.close();
            return false;
        }
        std::uint16_t bitsPerPixel = 0;
        if(!readBitmapInfoHeader(inputFile, bitsPerPixel)){
            inputFile.close();
            return false;
        }
        auto size = domain::graphics2d::mul_size(width_, height_);
        if(!size.has_value()){
            return false;
        }
        std::unique_ptr<domain::graphics2d::PixelBuffer> pixelBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(!pixelBuffer->isValid()){
            return false;
        }
        if(pixelBuffer_->size_ != size.value()){
            return false;
        }
        pixelBuffer_ = std::move(pixelBuffer);
        const size_t byteParPixel = bitsPerPixel / 8;
        for(size_t line = 0; line < height_; ++line){
            if(!readBitmapCollorBuffer(inputFile, byteParPixel, line)){
                pixelBuffer_ = nullptr;
                inputFile.close();
                break;
            }
        }
        inputFile.close();



        return false;
    }
    
    bool BMP::saveImage(const std::string& outputFilePath)const {
        return false;
    }

    bool BMP::readBitmapFileHeader(std::ifstream& infStream)const{
        char header[14];
        infStream.read(header, FILEHEADER_SIZE);
        if(header[0] != 'B' || header[1] != 'M'){
            // Not a valid BMP file
            return false;
        }
        return true;
    }

    bool BMP::readBitmapInfoHeader(std::ifstream& infStream, std::uint16_t& bitsPerPixel){
        char infoHeader[40];
        infStream.read(infoHeader, INFOHEADER_SIZE);
        std::uint32_t width = *reinterpret_cast<std::uint32_t*>(&infoHeader[4]);
        std::uint32_t height = *reinterpret_cast<std::uint32_t*>(&infoHeader[8]);
        if(width == 0 || height == 0){
            return false;
        }
        std::uint16_t bitsPerPixel = *reinterpret_cast<std::uint16_t*>(&infoHeader[14]);
        if(bitsPerPixel != 24 && bitsPerPixel !=32){
            return false;
        }
        std::uint32_t compression = *reinterpret_cast<std::uint32_t*>(&infoHeader[16]);
        if(compression != 0){
            return false;
        }
        width_ = static_cast<size_t>(width);
        height_ = static_cast<size_t>(height);
        return true;
    }

    bool BMP::readBitmapCollorBuffer(std::ifstream& infStream, const size_t& bytePerPixel, size_t lineNumber){
        if(!pixelBuffer_->isValid() || 
            lineNumber >= height_ || 
            bytePerPixel < 3 || bytePerPixel >4 || 
            width_ * bytePerPixel+1 > pixelBuffer_->size_){
            return false;
        }
        for(size_t col = (height_ - lineNumber-1) * width_; col < width_; ++col){
            char bgr[4] = {0};
            infStream.read(bgr, bytePerPixel);
            domain::graphics2d::Pixel pixel;
            pixel.b_ = static_cast<float>(static_cast<unsigned char>(bgr[0])) / 255.0f;
            pixel.g_ = static_cast<float>(static_cast<unsigned char>(bgr[1])) / 255.0f;
            pixel.r_ = static_cast<float>(static_cast<unsigned char>(bgr[2])) / 255.0f;
            size_t pos = lineNumber * width_ + col;
            domain::graphics2d::setPixel(*pixelBuffer_, pixel, pos);
        }
        // Skip padding bytes
        size_t rowSize = (width_ * 3 + 3) & (~3);
        size_t paddingSize = rowSize - (width_ * 3);
        infStream.ignore(paddingSize);
    }
}
