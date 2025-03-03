#pragma once

#include "util/macros.hpp"
#include <cstdint>


_NAMESPACE_EXGRAPHICS_BEGIN

struct Color {
    std::uint8_t r, g, b;
    constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b) : r(r), g(g), b(b){};
};

_NAMESPACE_EXGRAPHICS_END