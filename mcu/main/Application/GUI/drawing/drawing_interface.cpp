#include "drawing_interface.hpp"
#include "utf8_parser.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

void DrawingInterface::draw_circle_helper(Offset offset, Offset point, Color color, std::int16_t radius, CirclePart part) {
    if (part.topRight) {
        draw_pixel(offset + point.invert_y(), color);
        draw_pixel(offset + point.invert_x().swap_xy(), color);
    }
    if (part.topLeft) {
        draw_pixel(offset + point.invert_xy(), color);
        draw_pixel(offset + point.invert_xy().swap_xy(), color);
    }
    if (part.bottomRight) {
        draw_pixel(offset + point, color);
        draw_pixel(offset + point.swap_xy(), color);
    }
    if (part.bottomLeft) {
        draw_pixel(offset + point.invert_x(), color);
        draw_pixel(offset + point.invert_y().swap_xy(), color);
    }
}

void DrawingInterface::fill_circle_helper(Offset offset, Offset point, Color color, std::int16_t radius, CirclePart part) {
    if (part.topRight) {
        draw_vertical_line(offset + point.invert_y(), point.y + 1, color);
        draw_vertical_line(offset + point.invert_x().swap_xy(), point.x + 1, color);
    }
    if (part.topLeft) {
        draw_vertical_line(offset + point.invert_xy(), point.y + 1, color);
        draw_vertical_line(offset + point.invert_xy().swap_xy(), point.x + 1, color);
    }
    if (part.bottomRight) {
        draw_vertical_line(offset + Offset(point.x, 0), point.y + 1, color);
        draw_vertical_line(offset + Offset(point.y, 0), point.x + 1, color);
    }
    if (part.bottomLeft) {
        draw_vertical_line(offset + Offset(-point.x, 0), point.y + 1, color);
        draw_vertical_line(offset + Offset(-point.y, 0), point.x + 1, color);
    }
}

void DrawingInterface::draw_horizon_line(Offset offset, std::uint16_t width, Color color) {
    draw_line(offset, offset + Offset(width, 0), color);
}

void DrawingInterface::draw_vertical_line(Offset offset, std::uint16_t height, Color color) {
    draw_line(offset, offset + Offset(0, height), color);
}

void DrawingInterface::draw_line(Offset start, Offset end, Color color) {
    std::int16_t x1 = start.x, y1 = start.y, x2 = end.x, y2 = end.y;
    std::int16_t tmp;
    std::int16_t x, y;
    std::int16_t dx, dy;
    std::int16_t err;
    std::int16_t ystep;
    std::int8_t swapxy = 0;

    if (x1 > x2)
        dx = x1 - x2;
    else
        dx = x2 - x1;
    if (y1 > y2)
        dy = y1 - y2;
    else
        dy = y2 - y1;

    if (dy > dx) {
        swapxy = 1;
        tmp    = dx;
        dx     = dy;
        dy     = tmp;
        tmp    = x1;
        x1     = y1;
        y1     = tmp;
        tmp    = x2;
        x2     = y2;
        y2     = tmp;
    }
    if (x1 > x2) {
        tmp = x1;
        x1  = x2;
        x2  = tmp;
        tmp = y1;
        y1  = y2;
        y2  = tmp;
    }
    err = dx >> 1;
    if (y2 > y1)
        ystep = 1;
    else
        ystep = -1;
    y = y1;

    for (x = x1; x <= x2; x++) {
        if (swapxy == 0)
            draw_pixel(Offset(x, y), color);
        else
            draw_pixel(Offset(y, x), color);
        err -= (std::uint16_t)dy;
        if (err < 0) {
            y += (std::uint16_t)ystep;
            err += (std::uint16_t)dx;
        }
    }
}

void DrawingInterface::draw_rect(Offset offset, Size size, Color color) {
    Offset end = offset + size.to_offset();
    Offset curr;
    for (curr.y = offset.y; curr.y <= end.y; curr.y++)
        for (curr.x = offset.x; curr.x <= end.x; curr.x++)
            draw_pixel(curr, color);
}

void DrawingInterface::fill_rect(Offset offset, Size size, Color color) {
    Offset end = offset + size.to_offset();
    Offset curr;
    for (curr.y = offset.y; curr.y <= end.y; curr.y++)
        for (curr.x = offset.x; curr.x <= end.x; curr.x++)
            draw_pixel(curr, color);
}

void DrawingInterface::draw_rounded_rect(Offset offset, Size size, Color color, std::int16_t radius) {
    int16_t maxRadius = ((size.width < size.height) ? size.width : size.height) / 2;
    radius            = radius > maxRadius ? maxRadius : radius < 0 ? 0
                                                                    : radius;
    draw_line(offset + Offset(radius, 0),
              offset + Offset(size.width - radius, 0),
              color);
    draw_line(offset + Offset(radius, size.height - 1),
              offset + Offset(size.width - radius, size.height - 1),
              color);
    draw_line(offset + Offset(0, radius),
              offset + Offset(0, size.height - radius),
              color);
    draw_line(offset + Offset(size.width - 1, radius),
              offset + Offset(size.width - 1, size.height - radius),
              color);
    draw_circle(offset + Offset(radius, radius),
                color,
                radius,
                CirclePart(true, false, false, false));
    draw_circle(offset + Offset(size.width - radius - 1, radius),
                color,
                radius,
                CirclePart(false, true, false, false));
    draw_circle(offset + Offset(radius, size.height - radius - 1),
                color,
                radius,
                CirclePart(false, false, true, false));
    draw_circle(offset + Offset(size.width - radius - 1, size.height - radius - 1),
                color,
                radius,
                CirclePart(false, false, false, true));
}

void DrawingInterface::fill_rounded_rect(Offset offset, Size size, Color color, std::int16_t radius) {
    int16_t maxRadius = ((size.width < size.height) ? size.width : size.height) / 2;
    radius            = radius > maxRadius ? maxRadius : radius < 0 ? 0
                                                                    : radius;
    std::int16_t xl, yu, yl, xr, ww, hh;
    xl = offset.x + radius;
    yu = offset.y + radius;
    xr = offset.x + size.width - radius - 1;
    yl = offset.y + size.height - radius - 1;
    fill_circle(Offset(xl, yu), color, radius, CirclePart(true, false, false, false));
    fill_circle(Offset(xr, yu), color, radius, CirclePart(false, true, false, false));
    fill_circle(Offset(xl, yl), color, radius, CirclePart(false, false, true, false));
    fill_circle(Offset(xr, yl), color, radius, CirclePart(false, false, false, true));
    ww = size.width - 2 * radius;
    xl = xl + 1;
    yu = yu + 1;
    yl = yl + 1;
    if (ww >= 3) {
        ww = ww - 2;
        fill_rect(Offset(xl, offset.y), Size(ww, radius + 1), color);
        fill_rect(Offset(xl, yl), Size(ww, radius + 1), color);
    }
    hh = size.height - 2 * radius;
    if (hh >= 3) {
        hh = hh - 2;
        fill_rect(Offset(offset.x, yu), Size(size.width, hh), color);
    }
}

void DrawingInterface::draw_circle(Offset offset, Color color, std::uint16_t radius, CirclePart part) {
    int16_t f = 1 - radius;
    Offset ddF(1, -2 * radius);
    Offset cur(0, radius);
    draw_circle_helper(offset, cur, color, radius, part);
    while (cur.x < cur.y) {
        if (f >= 0) {
            cur.y--;
            ddF.y += 2;
            f += ddF.y;
        }
        cur.x++;
        ddF.x += 2;
        f += ddF.x;
        draw_circle_helper(offset, cur, color, radius, part);
    }
}

void DrawingInterface::fill_circle(Offset offset, Color color, std::uint16_t radius, CirclePart part) {
    std::int16_t f = 1 - radius;
    Offset ddF(1, -2 * radius);
    Offset cur(0, radius);
    fill_circle_helper(offset, cur, color, radius, part);
    while (cur.x < cur.y) {
        if (f >= 0) {
            cur.y--;
            ddF.y += 2;
            f += ddF.y;
        }
        cur.x++;
        ddF.x += 2;
        f += ddF.x;
        fill_circle_helper(offset, cur, color, radius, part);
    }
}

void DrawingInterface::draw_pill(Offset offset, Size size, Color color) {
    if (size.width < size.height)
        draw_rect(offset, size, color);
    int radius = size.height / 2;
    if (size.height % 2 == 0) {
        draw_circle(offset + Offset(radius, radius), color, radius, CirclePart(true, false, false, false));
        draw_circle(offset + Offset(radius, radius + 1), color, radius, CirclePart(false, false, true, false));
        draw_line(offset + Offset(radius + 1, 0), offset + Offset(1 + size.width - radius, 0), color);
        draw_line(offset + Offset(radius + 1, size.height - 1), offset + Offset(1 + size.width - radius, size.height - 1), color);
        draw_circle(offset + Offset(size.width - radius - 1, radius), color, radius, CirclePart(false, true, false, false));
        draw_circle(offset + Offset(size.width - radius - 1, radius + 1), color, radius, CirclePart(false, false, false, true));

    } else {
        draw_circle(offset + Offset(radius, radius), color, radius, CirclePart(true, false, true, false));
        draw_line(offset + Offset(radius + 1, 0), offset + Offset(1 + size.width - radius, 0), color);
        draw_line(offset + Offset(radius + 1, size.height - 1), offset + Offset(1 + size.width - radius, size.height - 1), color);
        draw_circle(offset + Offset(size.width - radius - 1, radius), color, radius, CirclePart(false, true, false, true));
    }
}

void DrawingInterface::fill_pill(Offset offset, Size size, Color color) {
    if (size.width < size.height)
        fill_rect(offset, size, color);
    int radius = size.height / 2;
    if (size.height % 2 == 0) {
        fill_circle(offset + Offset(radius, radius), color, radius, CirclePart(true, false, false, false));
        fill_circle(offset + Offset(radius, radius + 1), color, radius, CirclePart(false, false, true, false));
        fill_rect(offset + Offset(radius + 1, 0), Size(size.width - 2 * radius, size.height), color);
        fill_circle(offset + Offset(size.width - radius - 1, radius), color, radius, CirclePart(false, true, false, false));
        fill_circle(offset + Offset(size.width - radius - 1, radius + 1), color, radius, CirclePart(false, false, false, true));

    } else {
        fill_circle(offset + Offset(radius, radius), color, radius, CirclePart(true, false, true, false));
        fill_rect(offset + Offset(radius + 1, 0), Size(size.width - 2 * radius, size.height), color);
        fill_circle(offset + Offset(size.width - radius - 1, radius), color, radius, CirclePart(false, true, false, true));
    }
}

std::int16_t DrawingInterface::draw_character(Offset offset, const Font &font, const FontCharacter *ch, Color color) {
    if (ch == nullptr)
        return 0;
    offset.y += ch->offset_y;
    MonoBitmapReader reader(*ch, font.grayscale_bits);
    draw_bitmap(offset, reader, color);
    return ch->size.width;
}

std::int16_t DrawingInterface::draw_text(Offset offset, const Font &font, const char *text, Color color, const TextDrawingOption &option) {
    const char *str = text;
    std::uint32_t ch;
    std::size_t chLen;
    const FontCharacter *character;
    const FontCharacter *tailing = option._overflow == TextOverflow::Ellipsis ? font.find(UTF8Parser::next_char("…", nullptr)) : nullptr;
    std::int16_t remainWidth     = option._width;
    Offset row_inc, col_inc;

    switch (option._rotation) {
    case Rotation::rotate_0deg:
        row_inc = Offset(0, 1);
        col_inc = Offset(1, 0);
        break;
    case Rotation::rotate_90deg:
        row_inc = Offset(-1, 0);
        col_inc = Offset(0, 1);
        break;
    case Rotation::rotate_180deg:
        row_inc = Offset(0, -1);
        col_inc = Offset(-1, 0);
        break;
    case Rotation::rotate_270deg:
        row_inc = Offset(1, 0);
        col_inc = Offset(0, -1);
        break;
    default:
        return 0;
    }

    switch (option._ref_position) {
    case TextRefPosition::Top:
        break;
    case TextRefPosition::Center:
        offset = offset - row_inc * ((font.ascent_height + font.descent_height) / 2);
        break;
    case TextRefPosition::Baseline:
        offset = offset - row_inc * font.ascent_height;
        break;
    case TextRefPosition::Botton:
        offset = offset - row_inc * (font.ascent_height + font.descent_height);
        break;
    default:
        return 0;
    }
    switch (option._horizon_align) {
    case HorizonAlign::Start:
        break;
    default:
        std::int16_t text_width = get_text_width(font, text);
        if (text_width > option._width)
            break;
        switch (option._horizon_align) {
        case HorizonAlign::Center:
            offset = offset + col_inc * ((option._width - text_width) / 2);
            break;
        case HorizonAlign::End:
            offset = offset + col_inc * (option._width - text_width);
            break;
        default:
            break;
        }
    }

    Offset cur = offset;

    while ((ch = UTF8Parser::next_char(str, &chLen)) != 0) {
        str += chLen;
        character = font.find(ch);
        if (character == nullptr)
            continue;
        if (!tailing && remainWidth - character->size.width < 0) {
            return option._width - remainWidth;
        } else if (tailing && remainWidth - character->size.width < tailing->size.width) {
            MonoBitmapReader reader(*tailing, font.grayscale_bits);
            draw_bitmap(cur + (row_inc * tailing->offset_y), reader, color, option._rotation);
            cur = cur + (col_inc * tailing->size.width);
            remainWidth -= tailing->size.width;
            return option._width - remainWidth;
        }
        MonoBitmapReader reader(*character, font.grayscale_bits);
        draw_bitmap(cur + (row_inc * character->offset_y), reader, color, option._rotation);
        cur = cur + (col_inc * character->size.width);
        remainWidth -= character->size.width;
    }
    return option._width - remainWidth;
}

std::int16_t DrawingInterface::get_text_width(const Font &font, const char *text) {
    const char *str = text;
    std::uint32_t ch;
    std::size_t chLen;
    const FontCharacter *character;
    std::uint16_t width = 0;
    while ((ch = UTF8Parser::next_char(str, &chLen)) != 0) {
        str += chLen;
        character = font.find(ch);
        if (character == nullptr)
            continue;
        width += character->size.width;
    }
    return width;
}

_NAMESPACE_EXGRAPHICS_END