#pragma once

#include "hal_header.hpp"

#include <functional>

class Encoder {
  public:
    enum class EventType {
        increase,
        decrease
    };

    using CallbackFunction = void(EventType);

    static constexpr std::uint32_t debounce_delay_ms = 1;

  private:
    std::uint32_t last_triggered_time = 0;
    TIM_HandleTypeDef &handler;
    std::function<CallbackFunction> callback;

  public:
    inline Encoder(TIM_HandleTypeDef &tim_handler)
        : handler(tim_handler) {}

    void on_increase();
    void on_decrease();

    void init();
    void register_callback(std::function<CallbackFunction> cb);
};
