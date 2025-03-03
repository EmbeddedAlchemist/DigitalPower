#pragma once

#include "util/macros.hpp"

#include "bitmap.hpp"
#include "size.hpp"
#include <cstddef>
#include <cstdint>

_NAMESPACE_EXGRAPHICS_BEGIN

class MonoBitmap {
  private:
    friend class MonoBitmapReader;
    const std::uint32_t *data;

  public:
    const Size size;

    constexpr MonoBitmap(const std::uint32_t *data, Size size)
        : data(data), size(size) {}
};

class MonoBitmapReader {

    const std::size_t grayscale_bits;
    const std::size_t pixel_per_unit;
    const std::uint32_t read_mask;

    const std::uint32_t *read_ptr;
    std::size_t right_shift_count;

  public:
    const MonoBitmap &bitmap;
    constexpr MonoBitmapReader(const MonoBitmap &bitmap, std::size_t grayscale_bits)
        : bitmap(bitmap), grayscale_bits(grayscale_bits), read_ptr(bitmap.data),
          pixel_per_unit(sizeof(std::uint32_t) * 8 / grayscale_bits),
          right_shift_count(0), read_mask((1ul << grayscale_bits) - 1) {}

    inline void set_position(Offset offset) {
        std::size_t nth_pixel = offset.y * bitmap.size.width + offset.x;
        read_ptr              = bitmap.data + (nth_pixel / pixel_per_unit);
        right_shift_count     = (nth_pixel % pixel_per_unit) * grayscale_bits;
    }

    inline std::uint8_t read() {
        std::uint8_t result = ((*read_ptr) >> right_shift_count) & read_mask;
        right_shift_count += grayscale_bits;
        if (right_shift_count >= sizeof(std::uint32_t) * 8) {
            right_shift_count = 0;
            read_ptr += 1;
        }
        return result;
    }

    inline Size get_size() { return bitmap.size; }
    inline std::size_t get_grayscale_bits() { return grayscale_bits; }
};

template <std::size_t grayscale_bits>
class GratScaleBitmap : public MonoBitmap {
  public:
    operator MonoBitmapReader() {
        return MonoBitmapReader(*this, grayscale_bits);
    }
};

_NAMESPACE_EXGRAPHICS_END