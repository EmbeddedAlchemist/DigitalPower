#include "ui_resource.hpp"
#include "onboard_resource.hpp"
#include "graphics.hpp"



using namespace exgraphics;

extern osMessageQueueId_t uiInputEventQueueHandle;

Graphics<135, 240, DisplayBuffer<135, 30, RGB565>> graphics(lcd);
MainUI main_ui;
UIControler ui_controller(graphics);

void init_after_gui_thread_ready() {
    button1.register_callback([](Button::EventType event) {
        switch (event) {
        case Button::EventType::press:
            ui_controller.on_input(UI::InputEvent::button_1_press);
            break;
        case Button::EventType::release:
            ui_controller.on_input(UI::InputEvent::button_1_release);
            break;
        case Button::EventType::click:
            ui_controller.on_input(UI::InputEvent::button_1_click);
            break;
        case Button::EventType::hold:
            ui_controller.on_input(UI::InputEvent::button_1_hold);
            break;
        }
    });
    button2.register_callback([](Button::EventType event) {
        switch (event) {
        case Button::EventType::press:
            ui_controller.on_input(UI::InputEvent::button_2_press);
            break;
        case Button::EventType::release:
            ui_controller.on_input(UI::InputEvent::button_2_release);
            break;
        case Button::EventType::click:
            ui_controller.on_input(UI::InputEvent::button_2_click);
            break;
        case Button::EventType::hold:
            ui_controller.on_input(UI::InputEvent::button_2_hold);
            break;
        }
    });
    button_enc.register_callback([](Button::EventType event) {
        switch (event) {
        case Button::EventType::press:
            ui_controller.on_input(UI::InputEvent::button_enc_press);
            break;
        case Button::EventType::release:
            ui_controller.on_input(UI::InputEvent::button_enc_release);
            break;
        case Button::EventType::click:
            ui_controller.on_input(UI::InputEvent::button_enc_click);
            break;
        case Button::EventType::hold:
            ui_controller.on_input(UI::InputEvent::button_enc_hold);
            break;
        }
    });
    encoder.register_callback([](Encoder::EventType event) {
        switch (event) {
        case Encoder::EventType::decrease:
            ui_controller.on_input(UI::InputEvent::encoder_scroll_down);
            break;
        case Encoder::EventType::increase:
            ui_controller.on_input(UI::InputEvent::encoder_scroll_up);
            break;
        }
    });

    ui_controller.init(main_ui, uiInputEventQueueHandle);
}