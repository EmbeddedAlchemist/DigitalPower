#pragma once
#include "button.hpp"
#include "drawing_interface.hpp"
#include "encoder.hpp"

using namespace exgraphics;

class UI {
  public:
    enum class InputEvent : uint8_t {
        button_1_press,
        button_1_release,
        button_1_click,
        button_1_hold,
        button_2_press,
        button_2_release,
        button_2_click,
        button_2_hold,
        button_enc_press,
        button_enc_release,
        button_enc_click,
        button_enc_hold,
        encoder_scroll_up,
        encoder_scroll_down
    };

    virtual void on_input(InputEvent event)      = 0;
    virtual void on_mount(DrawingInterface *itf) = 0;
    virtual void on_unmount()                    = 0;
    virtual void before_draw()                   = 0;
    virtual void on_draw()                       = 0;
};