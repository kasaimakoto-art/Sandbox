/**
 * @file main.cpp
 * @brief エントリポイント。引数の表示と受け取りを行います。
 */
#include <iostream>
#include "../include/arguments.hpp"
#include "../../../infra/codecs/include/bmp.hpp"

/**
 * @brief アプリケーションのエントリポイント。
 */
int main(int argc, char* argv[]){
    Arguments args;
    args.showArguments(argc, argv);
    args.recieveArgument(argc, argv);
    kaf::infra::codecs::BMP bmpImage;
    if(args.getLoadBmpPath().empty()){
        std::cout << "No BMP path specified." << std::endl;
    } else {
        std::cout << "BMP Path: " << args.getLoadBmpPath() << std::endl;
        if(bmpImage.loadImage(args.getLoadBmpPath())){
            std::cout << "BMP image loaded successfully." << std::endl;
            std::cout << "Image Size: " << bmpImage.getWidth() << " x " << bmpImage.getHeight() << std::endl;
            std::cout << "Valid Image: " << (bmpImage.isValid() ? "Yes" : "No") << std::endl;
        } else {
            std::cout << "Failed to load BMP image." << std::endl;
        }
    }
    if(args.getSaveBmpPath().empty()){
        std::cout << "No BMP path specified." << std::endl;
    } else {
        std::cout << "BMP Path: " << args.getSaveBmpPath() << std::endl;
        if(bmpImage.saveImage(args.getSaveBmpPath(), 24/8)){
            std::cout << "BMP image saved successfully." << std::endl;
            std::cout << "Image Size: " << bmpImage.getWidth() << " x " << bmpImage.getHeight() << std::endl;
        } else {
            std::cout << "Failed to save BMP image." << std::endl;
        }
    }
    return 0;
}

