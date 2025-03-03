#pragma once

#include "Color.hpp"
#include <cstdint>

struct RGB565 {
  protected:
    std::uint8_t raw_r(void) const;
    std::uint8_t raw_g(void) const;
    std::uint8_t raw_b(void) const;

  public:
    std::uint16_t val;
    void r(uint8_t r);
    void g(uint8_t g);
    void b(uint8_t b);
    void rgb(uint8_t r, uint8_t g, uint8_t b);
    void hsv(float h, float s, float v);
    void hsl(float h, float s, float l);

    constexpr RGB565(uint8_t r, uint8_t g, uint8_t b)
        : val(((std::uint16_t)(r & 0xF8) << 8) | ((std::uint16_t)(g & 0xFC) << 3) | (b >> 3)) {}
    constexpr RGB565(uint16_t val) : val(val) {};
    constexpr RGB565(exgraphics::Color &color) : RGB565(color.r, color.g, color.b) {};
    RGB565(void) = default;

    /**
     * @brief 混色方法
     *
     * @param extra 待混合的颜色
     * @param extraRatio 待混合的颜色的占比 0~255
     * @return RGB565
     */
    RGB565 mix(RGB565 extra, std::uint8_t extra_ratio) const;
};
