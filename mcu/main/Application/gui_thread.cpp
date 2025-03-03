#include "gui_thread.hpp"
#include "Console.hpp"
#include "RGB565.hpp"
#include "cmsis_os.h"
#include "cstdio"
#include "display_buffer.hpp"
#include "font_DIN_COROS_56px_w500_gs2bit.hpp"
#include "font_DIN_COROS_64px_w500_gs2bit.hpp"
#include "font_Jetbrains_Mono_20px_w800_gs2bit.hpp"
#include "font_MiSans_10px_w700_gs4bit.hpp"
#include "font_MiSans_12px_w500_gs4bit.hpp"
#include "font_MiSans_16px_w500_gs2bit.hpp"
#include "font_MiSans_20px_w500_gs2bit.hpp"
#include "graphics.hpp"
#include "offset.hpp"
#include "onboard_resource.hpp"

using namespace exgraphics;

Graphics<135, 240, DisplayBuffer<135, 12, RGB565>> graphics(lcd);


constexpr Color voltage_color(0x72, 0xb8, 0xfd);
constexpr Color current_color(0xf8, 0xc6, 0x30);
constexpr Color power_color(0xec, 0x32, 0x5a);

auto &font_set_value        = font_MiSans_12px_w500_gs4bit;
auto &font_cur_value_number = font_DIN_COROS_56px_w500_gs2bit;
auto &font_cur_value_unit   = font_Jetbrains_Mono_20px_w800_gs2bit;
auto &font_btn_hint         = font_MiSans_12px_w500_gs4bit;

void draw_main_ui() {
    graphics.fill(RGB565(0, 0, 0));

    TextDrawingOption txt_opt_action_hint;
    txt_opt_action_hint
        .rotate(Rotation::rotate_270deg)
        .width(80);

    graphics.draw_text({134, 239}, font_btn_hint, "V-Set", {255, 255, 255}, TextDrawingOption(txt_opt_action_hint).horizon_align(HorizonAlign::Start));
    graphics.draw_text({134, 120}, font_btn_hint, "Settings", {255, 255, 255}, TextDrawingOption(txt_opt_action_hint).horizon_align(HorizonAlign::Center));
    graphics.draw_text({134, 79}, font_btn_hint, "I-Set", {255, 255, 255}, TextDrawingOption(txt_opt_action_hint).horizon_align(HorizonAlign::End));

    graphics.draw_text({0, 16}, font_set_value, "V-Set 12.30V", {255, 255, 255});
    auto rt_v_width = graphics.draw_text({0, 68}, font_cur_value_number, "12.16", voltage_color);
    graphics.draw_text(Offset(rt_v_width + 4, 68), font_cur_value_unit, "V", voltage_color);

    graphics.draw_text({0, 96}, font_set_value, "I-Set 3.00A", {255, 255, 255});
    auto rt_i_width = graphics.draw_text({0, 148}, font_cur_value_number, "00.15", current_color);
    graphics.draw_text(Offset(rt_i_width + 4, 148), font_cur_value_unit, "A", current_color);

    graphics.draw_text({0, 176}, font_set_value, "Power", {255, 255, 255});
    auto rt_p_width = graphics.draw_text({0, 228}, font_cur_value_number, "01.84", power_color);
    graphics.draw_text(Offset(rt_p_width + 4, 228), font_cur_value_unit, "W", power_color);
}

extern "C" void gui_thread(void *unused) {

    osThreadFlagsWait(gui_thread_signal_display_device_ready, osFlagsWaitAll, osWaitForever);

    auto last_ms = osKernelGetSysTimerCount() / (osKernelGetSysTimerFreq() / 1000);
    console.info("GUI thread start");

    graphics.first_page();
    do {
        graphics.fill(RGB565(0, 0, 0));
        graphics.update();
    } while (graphics.next_page());
    lcd.set_brightness(1.f);

    while (true) {
        auto diff_ms = osKernelGetSysTimerCount() / (osKernelGetSysTimerFreq() / 1000) - last_ms;
        char str_buf[32];
        // console.verbose("Frame rate %d", 1000 / diff_ms);
        last_ms = osKernelGetSysTimerCount() / (osKernelGetSysTimerFreq() / 1000);
        graphics.first_page();
        do {
            draw_main_ui();
            graphics.update();
        } while (graphics.next_page());
        // console.verbose("Frame updated.");
    }
}
