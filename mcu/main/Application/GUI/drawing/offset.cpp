#include "Offset.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN
Offset Offset::operator+(const Offset offset) const {
    return Offset(this->x + offset.x, this->y + offset.y);
}

Offset Offset::operator-(const Offset offset) const {
    return Offset(this->x - offset.x, this->y - offset.y);
}


Offset Offset::abs(void) const {
    return Offset(x > 0 ? x : -x, y > 0 ? y : -y);
}

Offset Offset::swap_xy(void) const {
    return Offset(y, x);
}

void Offset::swap_x_with(Offset &offset) {
    this->x = this->x ^ offset.x;
    offset.x = this->x ^ offset.x;
    this->x = this->x ^ offset.x;
}

void Offset::swap_y_with(Offset &offset) {
    this->y = this->y ^ offset.y;
    offset.y = this->y ^ offset.y;
    this->y = this->y ^ offset.y;
}

void Offset::swapWith(Offset &offset) {
    Offset tmp = offset;
    offset = *this;
    *this = tmp;
}

Offset Offset::invert_x(void) const {
    return Offset(-x, y);
}

Offset Offset::invert_y(void) const {
    return Offset(x, -y);
}

Offset Offset::invert_xy(void) const {
    return Offset(-x, -y);
}

Size Offset::to_size() const {
    return Size(x < 0 ? 0 : x + 1,
                y < 0 ? 0 : y + 1);
}

_NAMESPACE_EXGRAPHICS_END