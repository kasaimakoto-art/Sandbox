/**
 * @file bmp.hpp
 * @brief BMP 画像の読み書きユーティリティ。
 * @details 非圧縮(BI_RGB)の 24/32bpp を主対象とします。
 */
#ifndef __BMP_H__
#define __BMP_H__


#include <memory>
#include <string>
#include <fstream>

#include "../../../domain/graphics2d/include/image.hpp"
#include "../../../domain/graphics2d/include/pixel.hpp"
namespace kaf::infra::codecs{

    /**
     * @class BMP
     * @brief BMP 画像のロード/セーブを提供するクラス。
     * @details domain::graphics2d::Image を継承し、
     *          非圧縮(BI_RGB)の 24/32bpp を対象に読み書きを行います。
     */
    class BMP : public ::kaf::domain::graphics2d::Image {
    public:
        /** @brief BMP ファイルヘッダ（BITMAPFILEHEADER）のサイズ[バイト]。 */
        const size_t FILEHEADER_SIZE = 14;
        /** @brief DIB ヘッダ（BITMAPINFOHEADER）のサイズ[バイト]。 */
        const size_t INFOHEADER_SIZE = 40;

        const unsigned int BI_RGB = 0;

        /**
         * @brief 既定コンストラクタ。空の画像で初期化します。
         */
        BMP();

        /**
         * @brief 指定サイズで新規画像を初期化します。
         * @param width 画像の幅[px]
         * @param height 画像の高さ[px]
         */
        BMP(const size_t width, const size_t height, const kaf::domain::graphics2d::Pixel& pixel = kaf::domain::graphics2d::Pixel(1.0f,1.0f,1.0f));

        ~BMP();
        BMP(const BMP& other);
        BMP& operator=(const BMP& other);
        BMP(BMP&& other);
        BMP& operator=(BMP&& other);

        /**
         * @brief 既存の画素バッファを用いて画像を設定します。
         * @param pixels ピクセル配列
         * @param width 幅[px]
         * @param height 高さ[px]
         * @retval true 設定成功
         * @retval false 失敗（サイズ不一致 等）
         */
        bool setImage(const domain::graphics2d::PixelBuffer& pixels, size_t width, size_t height);

        /**
         * @brief BMP ファイルを読み込み、画像を構築します。
         * @param inputFilePath 入力ファイルパス
         * @retval true 読み込み成功
         * @retval false 失敗（ファイル不在、不正ヘッダ、未対応形式 等）
         */
        bool loadImage(const std::string& inputFilePath);

        /**
         * @brief 画像を BMP として保存します。
         * @param outputFilePath 出力ファイルパス
         * @retval true 保存成功
         * @retval false 失敗（画像未生成、書き込み失敗 等）
         */
        bool saveImage(const std::string& outputFilePath, const size_t bytePerPixel = 32)const;


    private:
        /**
         * @brief BITMAPFILEHEADER（先頭14バイト）を読み取り検証します。
         * @param infStream 入力ストリーム（バイナリ）
         * @retval true 検証成功（'BM' シグネチャ等）
         * @retval false 検証失敗
         */
        bool readBitmapFileHeader(std::ifstream& infStream)const;

        /**
         * @brief BITMAPINFOHEADER（40バイト）を読み取り検証します。
         * @param infStream 入力ストリーム（バイナリ）
         * @param bitsPerPixel 読み取ったビット深度(出力)
         * @retval true 検証成功（bpp、圧縮形式、幅高さ 等）
         * @retval false 検証失敗
         */
        bool readBitmapInfoHeader(std::ifstream& infStream, std::uint16_t& bitsPerPixel);

        /**
         * @brief ピクセル配列（カラーバッファ）を 1 行分読み込みます。
         * @param infStream 入力ストリーム（バイナリ）
         * @param bytePerPixel 1 ピクセル当たりのバイト数（3 or 4）
         * @param lineNumber 読み込み対象の行インデックス
         * @retval true 読み込み成功
         * @retval false 失敗（サイズ不一致、読み取りエラー 等）
         */
        bool readBitmapCollorBuffer(std::ifstream& infStream, const size_t& bytePerPixel, size_t lineNumber);

        bool writeBitmapFileHeader(std::ofstream& outfStream, const size_t& bytePerPixel)const;
        bool writeBitmapInfoHeader(std::ofstream& outfStream, const size_t& bytePerPixel)const;
        bool writeBitmapCollorBuffer(std::ofstream& outfStream, const size_t& bytePerPixel, size_t lineNumber)const;

    };

}
#endif
