#ifndef __BMP_H__
#define __BMP_H__


#include <memory>
#include <string>

#include "../../../domain/graphics2d/include/image.hpp"
#include "../../../domain/graphics2d/include/pixel.hpp"
namespace kaf::infra::codecs{

    class BMP {
    public:
        const size_t FILEHEADER_SIZE = 14;
        const size_t INFOHEADER_SIZE = 40;
        BMP(size_t width, size_t height);
        bool createNewImage(size_t width, size_t height, const domain::graphics2d::Pixel& backgroundColor = domain::graphics2d::Pixel(1.0f,1.0f,1.0f));
        bool loadImage(const std::string& inputFilePath);
        bool saveImage(const std::string& outputFilePath)const;
    private:
        std::unique_ptr<domain::graphics2d::Image> image_;

        bool readBitmapFileHeader(std::ifstream& infStream)const;
        bool readBitmapInfoHeader(std::ifstream& infStream)const;
        bool readBitmapCollorBuffer(std::ifstream& infStream, int lineNumber)const;

    };

}
#endif