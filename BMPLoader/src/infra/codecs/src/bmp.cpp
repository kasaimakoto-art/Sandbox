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
            fprintf(stderr, "Failed to open input file: %s\n", inputFilePath.c_str());
            inputFile.close();
            return false;
        }
        if(!readBitmapFileHeader(inputFile)){
            fprintf(stderr, "Failed to read BMP file header\n");
            inputFile.close();
            return false;
        }
        std::uint16_t bitsPerPixel = 0;
        if(!readBitmapInfoHeader(inputFile, bitsPerPixel)){
            fprintf(stderr, "Failed to read BMP info header\n");
            inputFile.close();
            return false;
        }
        auto size = domain::graphics2d::mul_size(getWidth(), getHeight());
        if(!size.has_value()){
            fprintf(stderr, "Invalid image size\n");
            inputFile.close();
            return false;
        }
        std::unique_ptr<domain::graphics2d::PixelBuffer> pixelBuffer = std::make_unique<domain::graphics2d::PixelBuffer>(size.value());
        if(!pixelBuffer->isValid()){
            fprintf(stderr, "Invalid pixel buffer\n");
            inputFile.close();
            return false;
        }
        if(pixelBuffer->size_ != size.value()){
            fprintf(stderr, "Pixel buffer size does not match expected size\n");
            inputFile.close();
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
            fprintf(stderr, "BMP image is not valid after loading\n");
            inputFile.close();
            return false;
        }
        inputFile.close();
        fprintf(stderr, "Successfully loaded BMP file: %s\n", inputFilePath.c_str());
        return true;
    }
    
    bool BMP::saveImage(const std::string& outputFilePath, const size_t bitPerPixel)const {
        if(getPixelBuffer() == nullptr || !getPixelBuffer()->isValid()){
            fprintf(stderr, "Invalid pixel buffer\n");
            return false;
        }
        const size_t bytePerPixel = bitPerPixel / 8;
        std::filesystem::path filePath(outputFilePath);
        if(std::filesystem::exists(filePath)) {return false;}
        std::ofstream outputFile(filePath.c_str(), std::ios::binary);
        if(!outputFile.is_open()){
            fprintf(stderr, "Failed to open output file: %s\n", outputFilePath.c_str());
            outputFile.close();
            return false;
        }

        if(!writeBitmapFileHeader(outputFile, bytePerPixel)) {
            fprintf(stderr, "Failed to write BMP file header\n");
            outputFile.close();
            return false;
        }

        if(!writeBitmapInfoHeader(outputFile, bytePerPixel)) {
            fprintf(stderr, "Failed to write BMP info header\n");
            outputFile.close();
            return false;
        }

        for(size_t line = 0; line < getHeight(); ++line) {
            if(!writeBitmapCollorBuffer(outputFile, bytePerPixel, line)) {
                fprintf(stderr, "Failed to write color buffer at line %zu\n", line);
                outputFile.close();
                return false;
            }
        }
        outputFile.close();
        fprintf(stderr, "Successfully saved BMP file: %s\n", outputFilePath.c_str());
        return true;
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
    bool BMP::writeBitmapFileHeader(std::ofstream& outfStream, const size_t& bytePerPixel)const{
        char header[14];
        header[0] = 'B';
        header[1] = 'M';
        std::uint32_t* fileSize = reinterpret_cast<std::uint32_t*>(&header[2]);
        *fileSize = FILEHEADER_SIZE + INFOHEADER_SIZE + getPixelBuffer()->size_ * bytePerPixel;
        std::uint16_t* reserved = reinterpret_cast<std::uint16_t*>(&header[6]);
        *reserved = 0;
        std::uint16_t* reserved2 = reinterpret_cast<std::uint16_t*>(&header[8]);
        *reserved2 = 0;
        std::uint32_t* offset = reinterpret_cast<std::uint32_t*>(&header[10]);
        *offset = FILEHEADER_SIZE + INFOHEADER_SIZE;
        outfStream.write(header, FILEHEADER_SIZE);
        return true;
    }
    bool BMP::writeBitmapInfoHeader(std::ofstream& outfStream, const size_t& bytePerPixel)const{
        char header[40];
        std::uint32_t* infoSize = reinterpret_cast<std::uint32_t*>(&header[0]);
        *infoSize = static_cast<std::uint32_t>(INFOHEADER_SIZE);
        std::uint32_t* width = reinterpret_cast<std::uint32_t*>(&header[4]);
        *width = static_cast<std::uint32_t>(getWidth());
        std::uint32_t* height = reinterpret_cast<std::uint32_t*>(&header[8]);
        *height = static_cast<std::uint32_t>(getHeight());
        std::uint16_t* planes = reinterpret_cast<std::uint16_t*>(&header[12]);
        *planes = 1;
        std::uint16_t* bitsPerPixel = reinterpret_cast<std::uint16_t*>(&header[14]);
        *bitsPerPixel = static_cast<std::uint16_t>(bytePerPixel * 8);
        std::uint32_t* compression = reinterpret_cast<std::uint32_t*>(&header[16]);
        *compression = BI_RGB;
        std::uint32_t* imageSize = reinterpret_cast<std::uint32_t*>(&header[20]);
        *imageSize = getPixelBuffer()->size_ * bytePerPixel;
        std::uint32_t* xPixelsPerMeter = reinterpret_cast<std::uint32_t*>(&header[24]);
        *xPixelsPerMeter = 0;
        std::uint32_t* yPixelsPerMeter = reinterpret_cast<std::uint32_t*>(&header[28]);
        *yPixelsPerMeter = 0;
        std::uint32_t* colorsUsed = reinterpret_cast<std::uint32_t*>(&header[32]);
        *colorsUsed = 0;
        std::uint32_t* colorsImportant = reinterpret_cast<std::uint32_t*>(&header[36]);
        *colorsImportant = 0;
        outfStream.write(header, INFOHEADER_SIZE);
        return true;
    }
    bool BMP::writeBitmapCollorBuffer(std::ofstream& outfStream, const size_t& bytePerPixel, size_t lineNumber)const{
        size_t verticalPos = getHeight() - lineNumber - 1;
        fprintf(stderr, "writing line %zu (vertical pos %zu)\n", lineNumber, verticalPos);
        for(size_t col = 0; col < getWidth(); ++col){
            const domain::graphics2d::Pixel* pixel = kaf::domain::graphics2d::Image::getPixel(col, verticalPos);
            char bgr[4] = {0};
            bgr[0] = static_cast<unsigned char>(pixel->b_ * 255.0f);
            bgr[1] = static_cast<unsigned char>(pixel->g_ * 255.0f);
            bgr[2] = static_cast<unsigned char>(pixel->r_ * 255.0f);
            bgr[3] = static_cast<unsigned char>(pixel->a_ * 255.0f);
            fprintf(stderr, "Pixel[%zu,%zu]: R=%x, G=%x, B=%x\n", col, verticalPos, static_cast<unsigned char>(bgr[2]), static_cast<unsigned char>(bgr[1]), static_cast<unsigned char>(bgr[0]));
            outfStream.write(bgr, bytePerPixel);
        }
        // Skip padding bytes
        size_t paddingSize =floor((bytePerPixel * getWidth() + 3)/4) * 4 - (bytePerPixel * getWidth());
        if(paddingSize == 0) return true;
        char* paddingData = new char[paddingSize];
        fprintf(stderr, "Writing %zu padding bytes\n", paddingSize);
        std::fill_n(paddingData, paddingSize, 0);
        outfStream.write(paddingData, paddingSize);
        delete[] paddingData;
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
        size_t paddingSize =floor((bytePerPixel * getWidth() + 3)/4) * 4 - (bytePerPixel * getWidth());
        if(paddingSize == 0) return true;
        fprintf(stderr, "Skipping %zu padding bytes\n", paddingSize);
        infStream.ignore(paddingSize);
            return true;
    }
}
