/**
 * @file pixel_buffer.hpp
 * @brief ピクセル配列（1次元バッファ）の所有と操作。
 */
#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#include <memory>
#include <vector>
#include "../include/pixel.hpp"

namespace kaf::domain::graphics2d{
    /**
     * @struct PixelBuffer
     * @brief 1 次元のピクセル配列を所有するバッファ。
     * @details 通常は画像の幅×高さの要素数を保持します。
     */
    struct PixelBuffer{
        /**
         * @brief 指定サイズでバッファを確保し、指定色で初期化します。
         * @param size ピクセル数（幅×高さ）
         * @param pixel 既定色（既定: 白）
         */
        PixelBuffer(const size_t size, const Pixel& pixel = Pixel(1.0f, 1.0f, 1.0f));
        /** @brief バッファが有効か（サイズ>0 かつ配列あり）。 */
        bool isValid() const { return size_ > 0 && pixels_ != nullptr; }
        /** @brief 要素数（ピクセル数）。 */
        size_t size_{};
        /** @brief ピクセル配列。 */
        std::unique_ptr<Pixel[]> pixels_{};
    };

    /**
     * @brief 連続したピクセルをコピー（配列→バッファ）。
     * @param target 対象バッファ
     * @param pixelArray 入力配列
     * @param size コピーする要素数
     * @param startPos 書き込み開始位置（既定 0）
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool setPixels(PixelBuffer& target, const std::unique_ptr<Pixel[]>& pixelArray, const size_t size, const size_t startPos = 0);

    /**
     * @brief 連続したピクセルをコピー（vector→バッファ）。
     * @param target 対象バッファ
     * @param pixelList 入力ベクタ
     * @param startPos 書き込み開始位置（既定 0）
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool setPixels(PixelBuffer& target, const std::vector<Pixel>& pixelList, const size_t startPos = 0);

    /**
     * @brief 1 要素を書き込み。
     * @param target 対象バッファ
     * @param pixel 値
     * @param pos 書き込み位置
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool setPixel(PixelBuffer& target, const Pixel& pixel, const size_t pos);

    /**
     * @brief 1 要素を読み出し。
     * @param buffer 入力バッファ
     * @param pixel 出力ピクセル
     * @param pos 取得位置
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool getPixel(const PixelBuffer& buffer, Pixel& pixel, const size_t pos);
}

#endif
