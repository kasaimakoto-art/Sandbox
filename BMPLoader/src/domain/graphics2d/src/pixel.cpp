/**
 * @file pixel.cpp
 * @brief Pixel の実装。
 */
#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    Pixel::Pixel()
        :r_(1.0f), g_(1.0f), b_(1.0f), a_(1.0f){}
    Pixel::Pixel(float r, float g, float b, float a)
        :r_(r), g_(g), b_(b), a_(a){}
    Pixel getWhitePixel(){ return Pixel(1.0f, 1.0f, 1.0f);}
}
