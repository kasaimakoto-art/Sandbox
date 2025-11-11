/**
 * @file bmp.cpp
 * @brief BMP 読み書きクラスの実装（進行中）。
 */
#include "../include/bmp.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <optional>
#include <cstdint>
#include <memory>

#include "../../../domain/graphics2d/include/image.hpp"
#include "../../../domain/graphics2d/include/pixel.hpp"
#include "../../../domain/graphics2d/include/pixel_buffer.hpp"

namespace kaf::infra::codecs{

    BMP::BMP(): ::kaf::domain::graphics2d::Image(){
        // Default constructor
    }

    BMP::BMP(const size_t width, const size_t height, const kaf::domain::graphics2d::Pixel& pixel):
        ::kaf::domain::graphics2d::Image(width, height, pixel){
    }

    BMP::~BMP(){
        setHeight(0);
        setWidth(0);
        setPixelBuffer(nullptr);
    }
    BMP::BMP(const BMP& other){
        if(other.getPixelBuffer() != nullptr){
            auto otherBuffer = other.getPixelBuffer();
            std::unique_ptr<domain::graphics2d::PixelBuffer> tmpBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(otherBuffer->size_);
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
    BMP& BMP::operator=(const BMP& other){
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
    BMP::BMP(BMP&& other){
        setHeight(other.getHeight());
        setWidth(other.getWidth());
        setPixelBuffer(other.passPixelBuffer());
    }
    BMP& BMP::operator=(BMP&& other){
        if(this == &other){
            return *this;
        }
        setHeight(other.getHeight());
        setWidth(other.getWidth());
        setPixelBuffer(other.passPixelBuffer());
        return *this;
    }

    bool BMP::setImage(const domain::graphics2d::PixelBuffer& pixelBuffer, size_t width, size_t height){
        std::optional<size_t> size = domain::graphics2d::mul_size(width, height);
        if(!size.has_value()) return false;
        if(!pixelBuffer.isValid()) return false;
        if(pixelBuffer.size_ != size.value()) return false;
        std::unique_ptr<domain::graphics2d::PixelBuffer> tmpBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(tmpBuffer == nullptr) return false;
        bool result = domain::graphics2d::setPixels(*tmpBuffer, pixelBuffer.pixels_, size.value(), 0);
        if(!result){
            tmpBuffer = nullptr;
            return false;
        }
        setPixelBuffer(std::move(tmpBuffer));
        setHeight(height);
        setWidth(width);
        return true;
    }

    bool BMP::loadImage(const std::string& inputFilePath){
        if(getPixelBuffer() != nullptr){
            setPixelBuffer(nullptr);
        }
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
        auto size = domain::graphics2d::mul_size(getWidth(), getHeight());
        if(!size.has_value()){
            return false;
        }
        std::unique_ptr<domain::graphics2d::PixelBuffer> pixelBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(!pixelBuffer->isValid()){
            return false;
        }
        if(pixelBuffer->size_ != size.value()){
            return false;
        }
        setPixelBuffer(std::move(pixelBuffer));
        const size_t byteParPixel = static_cast<size_t>(bitsPerPixel / 8);
        for(size_t line = 0; line < getHeight(); ++line){
            if(!readBitmapCollorBuffer(inputFile, byteParPixel, line)){
                fprintf(stderr, "Failed to read color buffer at line %zu\n", line);
                setPixelBuffer(nullptr);
                inputFile.close();
                break;
            }
            fprintf(stderr, "Successfully read line %zu\n", line);
        }
        if(!isValid()){
            return false;
        }
        inputFile.close();
        return true;
    }
    
    bool BMP::saveImage(const std::string& outputFilePath)const {
        return false;
    }

    bool BMP::readBitmapFileHeader(std::ifstream& infStream)const{
        char header[14];
        infStream.read(header, FILEHEADER_SIZE);
        fprintf(stderr, "BMP Header(%d & %d): %d | %d\n",'B', 'M', header[0], header[1]);
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
        fprintf(stderr, "BMP InfoHeader: width=%u, height=%u\n", width, height);
        if(width == 0 || height == 0){
            return false;
        }
        bitsPerPixel = *reinterpret_cast<std::uint16_t*>(&infoHeader[14]);
        fprintf(stderr, "BMP BitsPerPixel: %u\n", bitsPerPixel);
        if(bitsPerPixel != 24 && bitsPerPixel !=32){
            return false;
        }
        std::uint32_t compression = *reinterpret_cast<std::uint32_t*>(&infoHeader[16]);
        fprintf(stderr, "BMP Compression: %u\n", compression);
        if(compression != 0){
            return false;
        }
        setWidth(static_cast<size_t>(width));
        setHeight(static_cast<size_t>(height));
        return true;
    }

    bool BMP::readBitmapCollorBuffer(std::ifstream& infStream, const size_t& bytePerPixel, size_t lineNumber){
        if(!getPixelBuffer()->isValid() || 
            lineNumber >= getHeight() || 
            bytePerPixel < 3 || bytePerPixel >4 || 
            getWidth() * bytePerPixel+1 > getPixelBuffer()->size_){
            return false;
        }
        size_t verticalPos = getHeight() - lineNumber - 1;
        fprintf(stderr, "Reading line %zu (vertical pos %zu)\n", lineNumber, verticalPos);
        for(size_t col = 0; col < getWidth(); ++col){
            char bgr[4] = {0};
            infStream.read(bgr, bytePerPixel);
            domain::graphics2d::Pixel pixel;
            pixel.b_ = static_cast<float>(static_cast<unsigned char>(bgr[0])) / 255.0f;
            pixel.g_ = static_cast<float>(static_cast<unsigned char>(bgr[1])) / 255.0f;
            pixel.r_ = static_cast<float>(static_cast<unsigned char>(bgr[2])) / 255.0f;
            fprintf(stderr, "Pixel[%zu,%zu]: R=%f, G=%f, B=%f\n", col, verticalPos, pixel.r_, pixel.g_, pixel.b_);
            size_t pos = verticalPos * getWidth() + col;
            domain::graphics2d::setPixel(*getPixelBuffer(), pixel, pos);
        }
        // Skip padding bytes
        size_t rowSize = (getWidth() * 3 + 3) & (~3);
        size_t paddingSize = rowSize - (getWidth() * 3);
        infStream.ignore(paddingSize);
        return true;
    }
}
