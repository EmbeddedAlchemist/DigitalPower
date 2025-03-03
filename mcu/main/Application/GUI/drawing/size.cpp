#include "Size.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

std::size_t Size::get_area(void) const {
    return width * height;
}
Size Size::operator+(Offset offset) const {
    return Size(width + offset.x, height + offset.y);
}
Size Size::swap_width_height(void) const {
    return Size(height, width);
}
Offset Size::to_offset() const {
    return Offset(width - 1, height - 1);
}

_NAMESPACE_EXGRAPHICS_END
