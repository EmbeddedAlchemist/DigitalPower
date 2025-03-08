#pragma once

#include "hal_header.hpp"

class PowerControl {
  private:
    TIM_HandleTypeDef &tim_buck_handle;
    std::uint32_t tim_buck_channel;
    TIM_HandleTypeDef &tim_boost_handle;
    std::uint32_t tim_boost_channel;
    GPIO_TypeDef *gpio_port_output_ctl;
    std::uint32_t gpio_pin_output_ctl;

    void tim_sync_start();
    void tim_stop();

  public:
    inline constexpr PowerControl(
        TIM_HandleTypeDef &tim_buck_handle,
        std::uint32_t tim_buck_channel,
        TIM_HandleTypeDef &tim_boost_handle,
        std::uint32_t tim_boost_channel,
        GPIO_TypeDef *gpio_port_output_ctl,
        std::uint32_t gpio_pin_output_ctl)
        : tim_buck_handle(tim_buck_handle),
          tim_buck_channel(tim_buck_channel),
          tim_boost_handle(tim_boost_handle),
          tim_boost_channel(tim_boost_channel),
          gpio_port_output_ctl(gpio_port_output_ctl),
          gpio_pin_output_ctl(gpio_pin_output_ctl) {}

    void init();
    void set_output_state(bool enable);
};