#include <gtest/gtest.h>

#include <memory>

#include "../src/infra/codecs/include/bmp.hpp"
#include "../src/domain/graphics2d/include/pixel.hpp"

using namespace kaf;
TEST(BMP, CreateNewImageFillsBackground) {
  domain::graphics2d::Pixel white(1.f,1.f,1.f);
  std::unique_ptr<infra::codecs::BMP> bmp = std::make_unique<infra::codecs::BMP>(4,3, white);
  ASSERT_NE(bmp, nullptr);
  EXPECT_EQ(bmp->isValid(), true);

  // Image へアクセスできるAPIが無いなら、検証用getterを追加する
  // 例: const Image& image() const; を用意して at(x,y) で確認
  // EXPECT_FLOAT_EQ(bmp.image().at(1,1).r, 1.f);
}
