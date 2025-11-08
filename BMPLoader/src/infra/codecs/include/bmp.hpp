#ifndef __BMP_H__
#define __BMP_H__


#include <memory>
#include <string>

#include "../../../domain/graphics2d/include/image.hpp"
#include "../../../domain/graphics2d/include/pixel.hpp"
namespace kaf::infra::codecs{

    class BMP : public ::kaf::domain::graphics2d::Image {
    public:
        const size_t FILEHEADER_SIZE = 14;
        const size_t INFOHEADER_SIZE = 40;
        BMP();
        BMP(size_t width, size_t height);
        bool setImage(const domain::graphics2d::PixelBuffer& pixels, size_t width, size_t height);
        bool loadImage(const std::string& inputFilePath);
        bool saveImage(const std::string& outputFilePath)const;
    private:

        bool readBitmapFileHeader(std::ifstream& infStream)const;
        bool readBitmapInfoHeader(std::ifstream& infStream, std::uint16_t& bitsPerPixel);
        bool readBitmapCollorBuffer(std::ifstream& infStream, const size_t& bytePerPixel, size_t lineNumber);

    };

}
#endif