#include "../include/bmp.hpp"

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <wingdi.h>

namespace kaf::infra::codecs{

    BMP::BMP(size_t width, size_t height){
        createNewImage(width, height, domain::graphics2d::Pixel(1.0f, 1.0f, 1.0f, 1.0f));
    }

    bool BMP::createNewImage(size_t width, size_t height, const domain::graphics2d::Pixel& backgroundColor){
        if(image_->pixels_ != nullptr){ return false; }
        std::optional<size_t> size = domain::graphics2d::mul_size(width, height);
        if(size < 1)return false;
        std::vector<domain::graphics2d::Pixel> buffer(size.value());
        std::fill(buffer.begin(),buffer.end(),backgroundColor);
        image_ = ::kaf::domain::graphics2d::createImage(buffer, (size_t)size.value(), width, height);
        if(image_->pixels_ != nullptr) return true;
        return false;
    }
    bool BMP::loadImage(const std::string& inputFilePath){
        if(image_->pixels_ != nullptr){ return false; }
        std::filesystem::path filePath(inputFilePath);
        if(!std::filesystem::exists(filePath)) {return false;}
        std::ifstream inputFile(filePath.c_str(), std::ios::binary);
        if(!inputFile.is_open()){
            return false;
        }
        


        return false;
    }
    bool BMP::saveImage(const std::string& outputFilePath)const {
        return false;
    }
}