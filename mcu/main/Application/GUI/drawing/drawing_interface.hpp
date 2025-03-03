#include "color.hpp"
#include "font.hpp"
#include "mono_bitmap.hpp"
#include "offset.hpp"
#include "size.hpp"
#include "util/macros.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

enum class Rotation : std::uint8_t {
    rotate_0deg,
    rotate_90deg,
    rotate_180deg,
    rotate_270deg
};

enum class TextOverflow : std::uint8_t {
    Ellipsis,
    Cut
};

enum class TextRefPosition : std::uint8_t {
    Top,
    Center,
    Baseline,
    Botton
};

enum class HorizonAlign : std::uint8_t {
    Start,
    Center,
    End
};

struct TextDrawingOption {
    std::int16_t _width       = 32767;
    TextOverflow _overflow        = TextOverflow::Ellipsis;
    Rotation _rotation            = Rotation::rotate_0deg;
    TextRefPosition _ref_position = TextRefPosition::Baseline;
    HorizonAlign _horizon_align   = HorizonAlign::Start;

    inline TextDrawingOption &width(std::int16_t w) { return _width = w, *this; }
    inline TextDrawingOption &overflow(TextOverflow o) { return _overflow = o, *this; }
    inline TextDrawingOption &rotate(Rotation r) { return _rotation = r, *this; }
    inline TextDrawingOption &ref_position(TextRefPosition r) { return _ref_position = r, *this; }
    inline TextDrawingOption &horizon_align(HorizonAlign a) { return _horizon_align = a, *this; }
};

class DrawingInterface {
  public:
    struct CirclePart {
        bool topLeft : 1;
        bool topRight : 1;
        bool bottomLeft : 1;
        bool bottomRight : 1;
        inline CirclePart(bool topLeft, bool topRight, bool bottomLeft, bool bottomRight)
            : topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight) {}
    };

  private:
    void draw_circle_helper(Offset offset, Offset point, Color color, std::int16_t radius, CirclePart part);
    void fill_circle_helper(Offset offset, Offset point, Color color, std::int16_t radius, CirclePart part);

    void draw_horizon_line(Offset offset, std::uint16_t width, Color color);
    void draw_vertical_line(Offset offset, std::uint16_t height, Color color);

  public:
    // basic drawing
    virtual void draw_pixel(Offset offset, Color color) = 0;
    virtual void draw_line(Offset start, Offset end, Color color);

    virtual void draw_rect(Offset offset, Size size, Color color);
    virtual void fill_rect(Offset offset, Size size, Color color);

    // extra drawing
    virtual void draw_rounded_rect(Offset offset, Size size, Color color, std::int16_t radius);
    virtual void fill_rounded_rect(Offset offset, Size size, Color color, std::int16_t radius);
    virtual void draw_circle(Offset offset, Color color, std::uint16_t radius, CirclePart part = CirclePart(true, true, true, true));
    virtual void fill_circle(Offset offset, Color color, std::uint16_t radius, CirclePart part = CirclePart(true, true, true, true));
    virtual void draw_pill(Offset offset, Size size, Color color);
    virtual void fill_pill(Offset offset, Size size, Color color);

    // bitmap drawing
    virtual void draw_bitmap(Offset offset, MonoBitmapReader &bitmap, Color color, Rotation rotation = Rotation::rotate_0deg) = 0;

    // text
    std::int16_t draw_character(Offset offset, const Font &font, std::uint32_t utf8_character, Color color);
    std::int16_t draw_text(Offset offset, const Font &font, const char *text, Color color, const TextDrawingOption &option = TextDrawingOption());
    std::int16_t get_text_width(const Font &font, const char *text);

    // drawing control
    virtual bool is_in_drawable_area(Offset offset, Size size) = 0;
    virtual void set_clip_window(Offset offset, Size size)     = 0;
    virtual void get_clip_window(Offset &offset, Size &size)   = 0;
    virtual void reset_clip_window()                           = 0;
};

_NAMESPACE_EXGRAPHICS_END