#include "button.hpp"

void Button::on_press() {
    if(pressed)
        return;
    if (HAL_GetTick() - release_time < debounce_delay_ms) {
        debounce_ignore = true;
        return;
    }
    press_time      = HAL_GetTick();
    debounce_ignore = false;
    pressed         = true;
    if ((bool)callback)
        callback(EventType::press);
}

void Button::on_release() {
    if (debounce_ignore || pressed == false)
        return;
    pressed = false;
    release_time = HAL_GetTick();
    if ((bool)callback) {
        callback(EventType::release);
        if (hold_event_triggered == false) {
            callback(EventType::click);
        }
    }
    hold_event_triggered = false;
}

void Button::loop_handle() {
    if (pressed == false || hold_event_triggered == true || debounce_ignore == true)
        return;
    if (HAL_GetTick() - press_time >= hold_trigger_delay_ms) {
        if ((bool)callback)
            callback(EventType::hold);
        hold_event_triggered = true;
    }
}

void Button::register_callback(std::function<CallbackFunction> cb) {
    callback = cb;
}
