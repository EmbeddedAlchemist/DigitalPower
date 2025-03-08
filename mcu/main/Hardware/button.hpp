#pragma once

#include "hal_header.hpp"
#include <functional>

class Button {
  public:
    enum class EventType {
        click,
        hold,
        press,
        release
    };

    using CallbackFunction = void(EventType);

    static constexpr std::uint32_t hold_trigger_delay_ms = 300;
    static constexpr std::uint32_t debounce_delay_ms     = 10;

  private:
    std::uint32_t press_time   = 0;
    std::uint32_t release_time = 0;
    bool pressed               = false;
    bool hold_event_triggered  = false;
    bool debounce_ignore       = false;

    std::function<CallbackFunction> callback;

  public:
    void on_press();
    void on_release();
    void loop_handle();

    void register_callback(std::function<CallbackFunction> cb);
};