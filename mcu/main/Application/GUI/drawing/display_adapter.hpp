#pragma once

#include "util/macros.hpp"
#include "offset.hpp"
#include "size.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

template <typename tp_color_type>
class DisplayAdapter{
    public:
      using ColorType = tp_color_type;
      virtual void update_area(Offset offset, Size size, const ColorType *buffer) = 0;
};

_NAMESPACE_EXGRAPHICS_END