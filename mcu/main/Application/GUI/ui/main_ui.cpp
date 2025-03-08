#include "main_ui.hpp"
#include "clamp.hpp"
#include "onboard_resource.hpp"
#include <cctype>
#include <cmath>
#include <cstdio>

#include "font_MiSans_12px_w500_gs4bit.hpp"
#include "font_din_coros_56px_w500_gs1bit.hpp"
#include "font_jetbrains_mono_20px_w800_gs2bit.hpp"

static constexpr const Font
    &font_btn_hint         = font_MiSans_12px_w500_gs4bit,
    &font_status           = font_MiSans_12px_w500_gs4bit,
    &font_set_value        = font_MiSans_12px_w500_gs4bit,
    &font_cur_value_number = font_din_coros_56px_w500_gs1bit,
    &font_cur_value_unit   = font_jetbrains_mono_20px_w800_gs2bit;

static constexpr const Color
    color_voltage              = Color(103, 232, 249),
    color_voltage_leading_zero = Color(14, 116, 144),
    color_voltage_set          = Color(255, 255, 255),
    color_current              = Color(253, 224, 71),
    color_current_leading_zero = Color(161, 98, 7),
    color_current_set          = Color(255, 255, 255),
    color_power                = Color(253, 164, 175),
    color_power_leading_zero   = Color(190, 18, 60),
    color_power_set            = Color(255, 255, 255),
    color_input                = Color(192, 192, 192),
    color_btn_hint             = Color(192, 192, 192);

static constexpr std::int16_t width_real_value_number = 20;
static constexpr std::int16_t gap_set_real            = 4;

float ease_func(float x) {
    return clamp<float>(0.f, 1.f - std::pow(1.f - x, 5.f), 1.f);
}

std::int16_t MainUI::draw_real_number(Offset offset, Color color_normal, Color color_leading_zero, float value) {
    Offset offset_cur = offset;
    char str_buf[8];
    snprintf(str_buf, sizeof(str_buf), "%05.2f", value);
    bool drawing_leading_zero = true;

    for (const char *ch = str_buf; *ch != 0; ch++) {
        if (*ch != '0')
            drawing_leading_zero = false;
        auto character = font_cur_value_number.find(*ch);
        if (character == nullptr)
            continue;
        if (std::isdigit(*ch)) {
            itf->draw_character(
                offset_cur + Offset((width_real_value_number - character->size.width) / 2, 0),
                font_cur_value_number,
                character,
                drawing_leading_zero ? color_leading_zero : color_normal);
            offset_cur = offset_cur + Offset(width_real_value_number, 0);
        } else {
            offset_cur = offset_cur + Offset(itf->draw_character(offset_cur, font_cur_value_number, character, color_normal), 0);
        }
    }
    return offset_cur.x - offset.x;
}

void MainUI::draw_set_and_real_value(Offset offset, Color color_set, Color color_normal, Color color_leading_zero, const char *title, float set, float real, const char *unit) {
    char str_buf[16];
    Offset cur_offset = offset;

    cur_offset = cur_offset + Offset(0, font_set_value.ascent_height);
    snprintf(str_buf, sizeof(str_buf), "%s %05.2f%s", title, set, unit);
    itf->draw_text(
        cur_offset,
        font_set_value,
        str_buf,
        color_set);

    snprintf(str_buf, sizeof(str_buf), "%05.2f", real);

    auto rt_v_width = draw_real_number(cur_offset + Offset(0, gap_set_real), color_normal, color_leading_zero, real);
    cur_offset      = cur_offset + Offset(0, gap_set_real) + Offset(0, font_cur_value_number.ascent_height);

    itf->draw_text(
        cur_offset + Offset(rt_v_width + 4, 0),
        font_cur_value_unit,
        unit,
        color_normal);
}

void MainUI::draw_voltage() {
    draw_set_and_real_value(
        Offset(0, 35 + y_offset_main),
        color_voltage_set,
        color_voltage,
        color_voltage_leading_zero,
        "U-Set",
        voltage_set,
        voltage_real,
        "V");
}

void MainUI::draw_current() {
    draw_set_and_real_value(
        Offset(0, 105 + y_offset_main),
        color_current_set,
        color_current,
        color_current_leading_zero,
        "I-Set",
        current_set,
        current_real,
        "A");
}

void MainUI::draw_power() {
    if (hide_power)
        return;
    draw_set_and_real_value(
        Offset(0, 175 + y_offset_main),
        color_power_set,
        color_power,
        color_power_leading_zero,
        "Max",
        voltage_set * current_set,
        voltage_real * current_real,
        "W");
}

void MainUI::draw_input() {
    if (hide_input)
        return;
    TextDrawingOption txt_opt;
    txt_opt.ref_position(TextRefPosition::Top);
    itf->draw_text(
        Offset(0, y_offset_main),
        font_status,
        str_input_type,
        color_input,
        txt_opt);
    itf->draw_text(
        Offset(0, 14 + y_offset_main),
        font_status,
        str_input_voltage,
        color_input,
        txt_opt);
}

void MainUI::draw_selector() {
    if (hide_selector)
        return;
    Offset offset = Offset(0, 240 + y_offset_selector);
    // itf->set_clip_window(offset, Size(135, 100));
    itf->fill_rect(offset, Size(135, 100), Color(0, 0, 0));
    itf->draw_line(offset + Offset(0, 4), offset + Offset(115, 4), Color(128, 128, 128));
    itf->draw_text(offset + Offset(0, 8), font_set_value, str_selector_title, color_voltage_set, TextDrawingOption().ref_position(TextRefPosition::Top));
    Offset offset_centeral = offset + Offset(0, (100 - font_cur_value_number.ascent_height) / 2);
    {
        TextDrawingOption txt_opt;
        txt_opt.ref_position(TextRefPosition::Center);
        Offset offset_cur         = offset_centeral;
        bool drawing_leading_zero = true;
        for (std::size_t i = 0; i < sizeof(digit_array_selector) / sizeof(digit_array_selector[0]); i++) {
            if (digit_array_selector[i] != 0)
                drawing_leading_zero = false;
            char ch        = digit_array_selector[i] + 48;
            auto character = font_cur_value_number.find(ch);

            itf->draw_character(
                offset_cur + Offset((width_real_value_number - character->size.width) / 2, 0),
                font_cur_value_number,
                character,
                i == curr_focus_digit  ? color_selector_focus
                : drawing_leading_zero ? color_selector_leading_zero
                                       : color_selector_normal);
            // if (i == curr_focus_digit) {
            //     itf->draw_character(
            //         offset_cur + Offset((width_real_value_number - character->size.width) / 2, -font_cur_value_number.ascent_height),
            //         font_cur_value_number,
            //         font_cur_value_number.find((digit_array_selector[i] == 0 ? 9 : digit_array_selector[i] - 1) + 48),
            //         color_selector_digit_near);
            //     itf->draw_character(
            //         offset_cur + Offset((width_real_value_number - character->size.width) / 2, font_cur_value_number.ascent_height),
            //         font_cur_value_number,
            //         font_cur_value_number.find((digit_array_selector[i] == 9 ? 0 : digit_array_selector[i] + 1) + 48),
            //         color_selector_digit_near);
            // }
            offset_cur = offset_cur + Offset(width_real_value_number, 0);
            if (i == 1) {
                offset_cur = offset_cur + Offset(itf->draw_character(offset_cur, font_cur_value_number, font_cur_value_number.find('.'), color_selector_normal), 0);
            }
        }
    }
    // itf->reset_clip_window();
}

void MainUI::draw_btn_hint() {
    itf->draw_line(Offset(115, 0), Offset(115, 239), Color(128, 128, 128));
    TextDrawingOption txt_opt;
    txt_opt
        .rotate(Rotation::rotate_270deg)
        .ref_position(TextRefPosition::Botton)
        .width(240);
    itf->draw_text(Offset(135, 239), font_btn_hint, str_btn_2_hint, color_btn_hint, txt_opt.horizon_align(HorizonAlign::Start));
    itf->draw_text(Offset(135, 239), font_btn_hint, str_btn_enc_hint, color_btn_hint, txt_opt.horizon_align(HorizonAlign::Center));
    itf->draw_text(Offset(135, 239), font_btn_hint, str_btn_1_hint, color_btn_hint, txt_opt.horizon_align(HorizonAlign::End));
}

MainUI::MainUI() {
    timeout_selector_hide.register_callback([&]() {
        timeout_selector_hide_ani.start();
        hide_input = false;
        hide_power = false;
    });
    timeout_selector_hide_ani.register_callback([&]() {
        hide_selector = true;
    });
    timeout_selector_show_ani.register_callback([&]() {
        hide_input = true;
        hide_power = true;
    });
}

void MainUI::on_input(InputEvent event) {
    switch (event) {
    case InputEvent::button_1_click:
        if (timeout_selector_hide.is_running())
            break;
        timeout_selector_hide_ani.end();
        timeout_selector_hide.start();
        timeout_selector_show_ani.start();
        hide_selector = false;
        break;
    default:
        break;
    }
}

void MainUI::on_mount(DrawingInterface *itf) {
    console.debug("main ui mounted");
    this->itf = itf;
}

void MainUI::on_unmount() {
    console.debug("main ui unmounted");
}

void MainUI::before_draw() {
    timeout_selector_hide.loop_handler();
    timeout_selector_hide_ani.loop_handler();
    timeout_selector_show_ani.loop_handler();
    voltage_real = power_sample.get_out_voltage();
    current_real = power_sample.get_out_current();
    std::snprintf(str_input_type, sizeof(str_input_type), "PPS 20.0V 2.0A");
    std::snprintf(str_input_voltage, sizeof(str_input_voltage), "Input: %.2fV", power_sample.get_in_voltage());
    if (timeout_selector_show_ani.is_running()) {
        y_offset_main     = -35.f * ease_func(timeout_selector_show_ani.get_progress());
        y_offset_selector = -100.f * ease_func(timeout_selector_show_ani.get_progress());
    } else if (timeout_selector_hide_ani.is_running()) {
        y_offset_main     = -35.f * (1.f - ease_func(timeout_selector_hide_ani.get_progress()));
        y_offset_selector = -100.f * (1.f - ease_func(timeout_selector_hide_ani.get_progress()));
    }
}

void MainUI::on_draw() {
    draw_input();
    draw_voltage();
    draw_current();
    draw_power();
    draw_selector();
    draw_btn_hint();
}
