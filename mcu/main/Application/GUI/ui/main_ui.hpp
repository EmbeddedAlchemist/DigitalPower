#include "drawing_interface.hpp"
#include "font.hpp"
#include "timeout.hpp"
#include "ui.hpp"

using namespace exgraphics;

class MainUI : public UI {
  private:
    DrawingInterface *itf = nullptr;
    float
        voltage_real,
        current_real,
        voltage_set = 12.00,
        current_set = 3.00;

    char str_input_type[16];
    char str_input_voltage[16];

    std::int16_t y_offset_main     = 0;
    std::int16_t y_offset_selector = 0;

    bool hide_power    = false;
    bool hide_input    = false;
    bool hide_selector = true;

    Timeout timeout_selector_hide           = Timeout(5000);
    Timeout timeout_selector_show_ani       = Timeout(400);
    Timeout timeout_selector_hide_ani       = Timeout(400);
    Timeout timeout_selector_number_adj_ani = Timeout(200);
    Timeout timeout_selector_number_sel_ani = Timeout(200);

    std::uint8_t digit_array_selector[4] = {0, 3, 0, 0};
    std::int16_t curr_focus_digit        = 3;
    std::int16_t x_offset_select_box     = 0;
    Color color_selector_focus           = Color(254, 249, 195);
    Color color_selector_normal          = Color(253, 224, 71);
    Color color_selector_leading_zero    = Color(161, 98, 7);
    Color color_selector_digit_near      = Color(234, 179, 8);
    char str_selector_title[16]          = "Adjust I-Set";

    const char *str_btn_1_hint   = "V-Set";
    const char *str_btn_enc_hint = "Settings";
    const char *str_btn_2_hint   = "I-Set";

    std::int16_t draw_real_number(Offset offset, Color color_normal, Color color_leading_zero, float value);
    void draw_set_and_real_value(Offset offset, Color color_set, Color color_normal, Color color_leading_zero, const char *title, float set, float real, const char *unit);
    void draw_voltage();
    void draw_current();
    void draw_power();
    void draw_input();
    void draw_selector();
    void draw_btn_hint();

  public:
    MainUI();
    virtual void on_input(InputEvent event) override final;
    virtual void on_mount(DrawingInterface *itf) override final;
    virtual void on_unmount() override final;
    virtual void before_draw() override final;
    virtual void on_draw() override final;
};