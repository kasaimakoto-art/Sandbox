#ifndef __PIXEL_H__
#define __PIXEL_H__

namespace kaf::domain::graphics2d{
    struct Pixel{
        float r_, g_, b_, a_;
        Pixel();
        Pixel(float r, float g, float b, float a=1.0f);
    };
}
#endif