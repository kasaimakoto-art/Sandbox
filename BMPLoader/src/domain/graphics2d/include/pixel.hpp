/**
 * @file pixel.hpp
 * @brief RGBA ピクセル表現の定義。
 */
#ifndef __PIXEL_H__
#define __PIXEL_H__

namespace kaf::domain::graphics2d{
    /**
     * @struct Pixel
     * @brief 正規化 RGBA ピクセル（各成分 0.0〜1.0）。
     */
    struct Pixel{
        /** 赤成分（0.0〜1.0）/ 緑成分 / 青成分 / アルファ成分 */
        float r_, g_, b_, a_;
        /** @brief 既定コンストラクタ。全成分未初期化。 */
        Pixel();
        /**
         * @brief 各成分を指定してピクセルを生成します。
         * @param r 赤成分
         * @param g 緑成分
         * @param b 青成分
         * @param a アルファ成分（既定 1.0）
         */
        Pixel(float r, float g, float b, float a=1.0f);
    };
}
#endif
