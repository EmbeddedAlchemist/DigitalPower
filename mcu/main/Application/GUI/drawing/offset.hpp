#pragma once

#include "Size.hpp"
#include <cstdint>
#include <cstdlib>
#include "util/macros.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

class Size;

class Offset {
  public:
    std::int16_t x;
    std::int16_t y;

    constexpr Offset(std::int16_t x = 0, std::int16_t y = 0) : x(x), y(y){};

    Offset operator+(const Offset offset) const;
    Offset operator-(const Offset offset) const;

    template<typename NumericType>
    Offset operator*(NumericType f) const {
        return Offset(x*f, y*f);
    }

    Offset abs(void) const;
    Offset swap_xy(void) const;
    void swap_x_with(Offset &offset);
    void swap_y_with(Offset &offset);
    void swapWith(Offset &offset);

    Offset invert_x(void) const;
    Offset invert_y(void) const;
    Offset invert_xy(void) const;

    template <bool includeStart = true, bool includeEnd = true>
    bool in_area(const Offset start, const Offset end);
    Size to_size() const;
};


template <bool includeStart, bool includeEnd>
inline bool Offset::in_area(const Offset start, const Offset end) {
    bool resultStart, resultEnd;
    if (includeStart)
        resultStart = start.x <= x && start.y <= y;
    else
        resultStart = start.x < x && start.y < y;
    if (includeEnd)
        resultEnd = end.x >= x && end.y >= y;
    else
        resultEnd = end.x > x && end.y > y;
    return resultStart && resultEnd;
}

_NAMESPACE_EXGRAPHICS_END