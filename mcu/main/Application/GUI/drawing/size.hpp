#pragma once

#include <cstddef>
#include <cstdint>

#include "Offset.hpp"
#include "util/macros.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

class Offset;

class Size {
  public:
    std::int16_t width;
    std::int16_t height;

    constexpr Size(std::int16_t width = 0, std::int16_t height = 0) : width(width), height(height){};
    std::size_t get_area(void) const;
    Size operator+(Offset offset) const;
    Size swap_width_height(void) const;

    Offset to_offset() const;
};


_NAMESPACE_EXGRAPHICS_END