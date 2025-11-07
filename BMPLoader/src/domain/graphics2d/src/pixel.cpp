#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    Pixel::Pixel(float r, float g, float b, float a)
        :r_(r), g_(g), b_(b), a_(a){}
    Pixel getWhitePixel(){ return Pixel(1.0f, 1.0f, 1.0f);}
}