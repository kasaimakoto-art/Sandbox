/**
 * @file image.hpp
 * @brief 2D 画像（幅・高さとピクセルバッファ）の表現。
 */
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <optional>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

#include "pixel.hpp"
#include "pixel_buffer.hpp"


namespace kaf::domain::graphics2d{
    /**
     * @class Image
     * @brief ピクセルバッファを所有する 2D 画像。
     */
    class Image{
    public:
        /** @brief 既定コンストラクタ。 */
        Image();
        /**
         * @brief 画像を生成します。
         * @param buffer ピクセル配列（所有権を移動）
         * @param width 幅[px]
         * @param height 高さ[px]
         */
        Image(std::unique_ptr<PixelBuffer>&& buffer, size_t width, size_t height);

        Image(const size_t width, const size_t height, const Pixel& pixel = Pixel(1.0f, 1.0f, 1.0f));

        ~Image();
        Image(const Image& other);
        Image& operator=(const Image& other);
        Image(Image&& other);
        Image& operator=(Image&& other);

        /** @brief 画像・バッファが妥当かを検証します。 */
        bool isValid() const;

        size_t getWidth() const { return width_; }
        void setWidth(size_t width) { width_ = width; }
        size_t getHeight() const { return height_; }
        void setHeight(size_t height) { height_ = height; }
        PixelBuffer* getPixelBuffer() const { return pixelBuffer_.get(); }
        std::unique_ptr<PixelBuffer> passPixelBuffer() { return std::move(pixelBuffer_); }
        void setPixelBuffer(std::unique_ptr<PixelBuffer>&& buffer) { pixelBuffer_ =  std::move(buffer); }

        Pixel* getPixel(const size_t width, const size_t height);
        bool setPixel(const size_t width, const size_t height, const Pixel& pixel);

    private:
        /** 幅[px] */
        size_t width_{};
        /** 高さ[px] */
        size_t height_{};
        /** ピクセルバッファ */
        std::unique_ptr<PixelBuffer> pixelBuffer_ = nullptr;
    };

    /**
     * @brief 幅×高さの積を安全に計算します。
     * @return 正常なら積、オーバーフロー時は std::nullopt、ゼロ含みなら 0。
     */
    inline std::optional<size_t> mul_size(size_t width, size_t height) {
        if(width == 0 || height == 0) return 0;
        if(width > std::numeric_limits<size_t>::max() / height) return std::nullopt;
        return width * height;
    }

    /**
     * @brief 指定位置のピクセルを取得します。
     * @param image 入力画像
     * @param pixel 出力ピクセル
     * @param width X 座標
     * @param height Y 座標
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool getPixel(const Image& image, Pixel& pixel, unsigned int width, unsigned int height);
    /**
     * @brief 指定位置のピクセルを書き込みます。
     * @param image 入力画像
     * @param pixel 値
     * @param width X 座標
     * @param height Y 座標
     * @retval true 成功
     * @retval false 失敗（範囲外 等）
     */
    bool setPixel(Image& image, const Pixel& pixel, unsigned int width, unsigned int height);
    /**
     * @brief ピクセルバッファから画像を生成します。
     * @param buffer ピクセル配列（所有権を移動）
     * @param width 幅[px]
     * @param height 高さ[px]
     * @return 生成された画像（失敗時 nullptr）
     */
    std::unique_ptr<Image> createImage(std::unique_ptr<PixelBuffer>&& buffer, size_t width, size_t height);
    /**
     * @brief ピクセルバッファをコピーして画像を生成します。
     * @param buffer 読み取り専用のピクセル配列
     * @param width 幅[px]
     * @param height 高さ[px]
     * @return 生成された画像（失敗時 nullptr）
     */
    std::unique_ptr<Image> copyImage(const PixelBuffer& buffer, size_t width, size_t height);

    /**
     * @brief ピクセルバッファから画像を生成します。
     * @param pixels 読み取り専用のピクセル配列
     * @param width 幅[px]
     * @param height 高さ[px]
     * @return 生成された画像（失敗時 nullptr）
     */
    std::unique_ptr<Image> createImage(const std::vector<Pixel>& pixels, size_t width, size_t height);

    bool createNewImage(const size_t width, const size_t height, const kaf::domain::graphics2d::Pixel& color);
}

#endif
