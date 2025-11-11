
#include <vector>
#include <memory>
#include <gtest/gtest.h>

#include "../src/domain/graphics2d/include/image.hpp"
#include "../src/domain/graphics2d/include/pixel_buffer.hpp"

using namespace kaf::domain::graphics2d;

TEST(ImageCreate, AcceptsExactSize) {
  const size_t w = 4, h = 3, n = w*h;
  std::vector<Pixel> buf(n, Pixel(0.2f,0.3f,0.4f));
  std::unique_ptr<PixelBuffer> pxBuf = std::make_unique<PixelBuffer>(n);
  setPixels(*pxBuf.get(), buf );
  auto img = createImage(std::move(pxBuf), w, h); // vector受けの版を用意済み前提
  ASSERT_NE(img, nullptr);
  EXPECT_EQ(img->getWidth(),  w);
  EXPECT_EQ(img->getHeight(), h);
  EXPECT_FLOAT_EQ(img->getPixel(0,0)->r_, 0.2f);
}

TEST(ImageCreate, RejectsInsufficientBuffer) {
  const size_t w = 4, h = 3;
  std::vector<Pixel> buf(5); // 足りない
  auto img = createImage(buf, w, h);
  EXPECT_EQ(img, nullptr);
}

TEST(ImageCreate, RejectsOverflow) {
  // mul_size がオーバーフロー検知するか
  const size_t big = std::numeric_limits<size_t>::max();
  std::vector<Pixel> dummy(1);
  auto img = createImage(dummy, big, big);
  EXPECT_EQ(img, nullptr);
}