#pragma once

#include "offset.hpp"
#include "util/macros.hpp"
#include <cstddef>
_NAMESPACE_EXGRAPHICS_BEGIN

template <std::size_t tp_width, std::size_t tp_height, typename tp_color_type>
struct DisplayBuffer {

    using ColorType                     = tp_color_type;
    static constexpr std::size_t width  = tp_width;
    static constexpr std::size_t height = tp_height;

    ColorType buffer[tp_height][tp_width];

    inline ColorType &offset(Offset offset) noexcept {
        if (offset.x >= width || offset.y >= height)
            asm("BKPT 0");
        return buffer[offset.y][offset.x];
    }
};

_NAMESPACE_EXGRAPHICS_END