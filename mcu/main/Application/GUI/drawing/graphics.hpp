#pragma once

#undef _LIBCPP_ABI_VCRUNTIME

#include "Offset.hpp"
#include "RGB565.hpp"
#include "display_adapter.hpp"
#include "display_buffer.hpp"
#include "drawing_interface.hpp"
#include "size.hpp"
#include "util/macros.hpp"
#include <algorithm>
#include <limits>

_NAMESPACE_EXGRAPHICS_BEGIN

template <std::size_t tp_display_width, std::size_t tp_display_height, typename tp_buffer_type>
class Graphics : public DrawingInterface {

  public:
    using BufferType = tp_buffer_type;
    using ColorType  = typename BufferType::ColorType;

  private:
    BufferType buffer;
    DisplayAdapter<ColorType> &display_adapter;
    Size display_size  = Size(tp_display_width, tp_display_height);
    Size page_size     = Size(BufferType::width, BufferType::height);
    Offset page_offset = Offset(0, 0);
    Offset clip_start  = Offset(0, 0);
    Offset clip_end    = Offset(tp_display_width, tp_display_height);

    inline bool in_display_area(Offset offset) {
        return offset.in_area<true, true>(Offset(0, 0), display_size.to_offset());
    }

    inline bool in_page_area(Offset offset) {
        return offset.in_area<true, true>(page_offset, page_offset + page_size.to_offset());
    }

    inline bool in_window_area(Offset offset) {
        return offset.in_area<true, false>(clip_start, clip_end);
    }

    inline bool in_drawable_area(Offset offset) {
        bool page    = in_page_area(offset),
             window  = in_window_area(offset),
             display = in_display_area(offset);
        return page && window && display;
    }

    inline bool contain_page_area(Offset target_start, Size size) {
        Offset page_end   = page_offset + page_size.to_offset(),
               target_end = target_start + size.to_offset();
        return page_end.y > target_start.y &&
               page_offset.y <= target_end.y &&
               page_end.x > target_start.x &&
               page_offset.x <= target_end.x;
    }

    inline bool contain_window_area(Offset target_start, Size size) {
        Offset target_end = target_start + size.to_offset();
        return clip_end.y > target_start.y &&
               clip_start.y <= target_end.y &&
               clip_end.x > target_start.x &&
               clip_start.x <= target_end.x;
    }

    inline bool contain_drawable_area(Offset offset, Size size) {
        bool page   = contain_page_area(offset, size),
             window = contain_window_area(offset, size);
        return page && window;
    }

    inline Offset limit_at_display_area(Offset offset) {
        if (offset.x < 0)
            offset.x = 0;
        else if (offset.x >= display_size.width)
            offset.x = display_size.width - 1;
        if (offset.y < 0)
            offset.y = 0;
        else if (offset.y >= display_size.height)
            offset.y = display_size.height - 1;
        return offset;
    }

    inline Offset limit_at_page_area(Offset offset) {
        if (offset.x < page_offset.x)
            offset.x = page_offset.x;
        else if (offset.x >= page_offset.x + page_size.width)
            offset.x = page_offset.x + page_size.width - 1;
        if (offset.y < page_offset.y)
            offset.y = page_offset.y;
        else if (offset.y >= page_offset.y + page_size.height)
            offset.y = page_offset.y + page_size.height - 1;
        return offset;
    }

    inline Offset limit_at_window_area(Offset offset) {
        if (offset.x < clip_start.x)
            offset.x = clip_start.x;
        else if (offset.x >= clip_end.x)
            offset.x = clip_end.x - 1;
        if (offset.y < clip_start.y)
            offset.y = clip_start.y;
        else if (offset.y >= clip_end.y)
            offset.y = clip_end.y - 1;
        return offset;
    }

    inline Offset limit_at_drawable_area(Offset offset) {
        offset = limit_at_page_area(offset);
        offset = limit_at_window_area(offset);
        offset = limit_at_display_area(offset);
        return offset;
    }

    inline Offset to_page_offset(Offset offset) {
        return offset - page_offset;
    }

    inline void draw_horizon_line(Offset start, Offset end, ColorType color) {
        if (start.x > end.x)
            start.swap_x_with(end);
        if (!contain_drawable_area(start, Size(end.x - start.x + 1, end.y - start.y + 1)))
            return;
        start = limit_at_drawable_area(start);
        end   = limit_at_drawable_area(end);
        start = to_page_offset(start);
        end   = to_page_offset(end);
        while (start.x <= end.x) {
            buffer.offset(start.x) = color;
            start.x++;
        }
    }

    inline void draw_vertical_line(Offset start, Offset end, ColorType color) {
        if (start.y > end.y)
            start.swap_y_with(end);
        if (!contain_drawable_area(start, Size(end.x - start.x + 1, end.y - start.y + 1)))
            return;
        start = limit_at_drawable_area(start);
        end   = limit_at_drawable_area(end);
        start = to_page_offset(start);
        end   = to_page_offset(end);
        while (start.y <= end.y) {
            buffer.offset(start) = color;
            start.y++;
        }
    }

  public:
    constexpr inline Graphics(DisplayAdapter<ColorType> &display_adapter)
        : display_adapter(display_adapter),
          buffer() {}

    inline virtual void draw_pixel(Offset offset, Color _color) override final {
        if (!in_drawable_area(offset))
            return;
        ColorType color(_color);
        buffer.offset(to_page_offset(offset)) = color;
    }

    // inline virtual void draw_line(Offset start, Offset end, Color _color) override final {
    //     ColorType color(_color);
    //     if (start.y == end.y)
    //         return draw_horizon_line(start, end, color);
    //     else if (start.x == end.x)
    //         return draw_vertical_line(start, end, color);
    //     DrawingInterface::draw_line(start, end, _color);
    // }

    inline virtual void fill_rect(Offset start, Size size, Color _color) override final {
        if (!contain_drawable_area(start, size))
            return;
        Offset end = start + size.to_offset();
        ColorType color(_color);
        start       = limit_at_drawable_area(start);
        start       = to_page_offset(start);
        end         = limit_at_drawable_area(end);
        end         = to_page_offset(end);
        Offset curr = start;
        for (curr.y = start.y; curr.y <= end.y; curr.y++)
            for (curr.x = start.x; curr.x <= end.x; curr.x++)
                buffer.offset(curr) = color;
    }

    virtual void draw_bitmap(Offset offset, MonoBitmapReader &reader, Color _color, Rotation rotation = Rotation::rotate_0deg) override final {
        Offset draw_start, draw_end;

        switch (rotation) {
        case Rotation::rotate_0deg:
            draw_start = offset;
            draw_end   = Offset(offset.x + reader.bitmap.size.width - 1, offset.y + reader.bitmap.size.height - 1);
            break;
        case Rotation::rotate_90deg:
            draw_start = Offset(offset.x - reader.bitmap.size.height + 1, offset.y);
            draw_end   = Offset(offset.x, offset.y + reader.bitmap.size.width - 1);
            break;
        case Rotation::rotate_180deg:
            draw_start = Offset(offset.x - reader.bitmap.size.width + 1, offset.y - reader.bitmap.size.height + 1);
            draw_end   = offset;
            break;
        case Rotation::rotate_270deg:
            draw_start = Offset(offset.x, offset.y - reader.bitmap.size.width + 1);
            draw_end   = Offset(offset.x + reader.bitmap.size.height - 1, offset.y);
            break;
        default:
            return;
        }

        if (!contain_drawable_area(draw_start, (draw_end - draw_start).to_size()))
            return;

        Offset limited_start = limit_at_drawable_area(draw_start);
        Offset limited_end   = limit_at_drawable_area(draw_end);

        Offset bitmap_start, bitmap_end, bitmap_offset, row_inc, col_inc;

        switch (rotation) {
        case Rotation::rotate_0deg:
            bitmap_start  = limited_start;
            bitmap_end    = limited_end;
            bitmap_offset = limited_start - draw_start;
            row_inc       = Offset(0, 1);
            col_inc       = Offset(1, 0);
            break;
        case Rotation::rotate_90deg:
            bitmap_start  = Offset(limited_end.x, limited_start.y);
            bitmap_end    = Offset(limited_start.x, limited_end.y);
            bitmap_offset = Offset(limited_start.y - draw_start.y, draw_end.x - limited_end.x);
            row_inc       = Offset(-1, 0);
            col_inc       = Offset(0, 1);
            break;
        case Rotation::rotate_180deg:
            bitmap_start  = limited_end;
            bitmap_end    = limited_start;
            bitmap_offset = Offset(draw_end.x - limited_end.x, draw_end.y - limited_end.y);
            row_inc       = Offset(0, -1);
            col_inc       = Offset(-1, 0);
            break;
        case Rotation::rotate_270deg:
            bitmap_start  = Offset(limited_start.x, limited_end.y);
            bitmap_end    = Offset(limited_end.x, limited_start.y);
            bitmap_offset = Offset(draw_end.y - limited_end.y, limited_start.x - draw_start.x);
            row_inc       = Offset(1, 0);
            col_inc       = Offset(0, -1);
            break;
        default:
            return;
        }

        ColorType color(_color);
        bitmap_start  = to_page_offset(bitmap_start);
        bitmap_end    = to_page_offset(bitmap_end);
        limited_start = to_page_offset(limited_start);
        limited_end   = to_page_offset(limited_end);

        Offset row_offset, col_offset;
        std::int16_t row            = 0;
        std::uint8_t gray_scale_max = (1u << reader.get_grayscale_bits()) - 1;
        while ((bitmap_start + row_offset).in_area<true, true>(limited_start, limited_end)) {
            col_offset = Offset(0, 0);
            reader.set_position(Offset(bitmap_offset.x, bitmap_offset.y + row));
            while ((bitmap_start + col_offset).in_area<true, true>(limited_start, limited_end)) {
                ColorType &pix = buffer.offset(bitmap_start + row_offset + col_offset);
                pix            = pix.mix(color, std::numeric_limits<std::uint8_t>::max() * reader.read() / gray_scale_max);
                col_offset     = col_offset + col_inc;
            }
            row_offset = row_offset + row_inc;
            row++;
        }
    }

    inline virtual bool is_in_drawable_area(Offset offset, Size size) override final {
        return is_in_drawable_area(offset, size);
    }

    inline virtual void set_clip_window(Offset offset, Size size) override final {
        clip_start = offset;
        clip_end   = offset + Offset(size.width, size.height);
    }

    inline virtual void get_clip_window(Offset &offset, Size &size) override final {
        offset = clip_start;
        size   = Size(clip_end.x - clip_start.x, clip_end.y - clip_start.y);
    }

    inline virtual void reset_clip_window() override final {
        clip_start = Offset(0, 0);
        clip_end   = Offset(display_size.width, display_size.height);
    }

    inline virtual void first_page(void) override final {
        page_offset = Offset(0, 0);
    }

    inline virtual bool next_page(void) override final {
        Offset bkup = page_offset;
        page_offset.x += page_size.width;
        if (page_offset.x < display_size.width)
            return true;
        page_offset.x = 0;
        page_offset.y += page_size.height;
        if (page_offset.y < display_size.height)
            return true;
        page_offset = bkup; // keep pageoffset;
        return false;
    }

    inline void fill(ColorType color) {
        std::fill_n(&buffer.offset({0, 0}), buffer.width * buffer.height, color);
    }

    inline virtual void update() override final {
        display_adapter.update_area(page_offset, page_size, &buffer.offset(Offset(0, 0)));
    }

    inline virtual void clear() override final {
        fill(ColorType(0,0,0));
    }

    inline virtual void set_brightness(float brightness) override final {
        display_adapter.set_brightness(brightness);
    }
};

_NAMESPACE_EXGRAPHICS_END