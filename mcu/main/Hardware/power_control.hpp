#pragma once

#include "hal_header.hpp"
#include "low_pass_filter.hpp"
#include "pid.hpp"

class PowerControl {
  public:
    using OutputVoltageGetter = float (*)();

  private:
    TIM_HandleTypeDef &tim_buck_handle;
    std::uint32_t tim_buck_channel;
    TIM_HandleTypeDef &tim_boost_handle;
    std::uint32_t tim_boost_channel;
    GPIO_TypeDef *gpio_port_output_ctl;
    std::uint32_t gpio_pin_output_ctl;
    PID pid_filter;
    OutputVoltageGetter output_voltage_getter;

    void tim_sync_start();
    void tim_stop();
    void set_output_value(float output);

  public:
    inline PowerControl(
        TIM_HandleTypeDef &tim_buck_handle,
        std::uint32_t tim_buck_channel,
        TIM_HandleTypeDef &tim_boost_handle,
        std::uint32_t tim_boost_channel,
        GPIO_TypeDef *gpio_port_output_ctl,
        std::uint32_t gpio_pin_output_ctl,
        OutputVoltageGetter output_voltage_getter)
        : tim_buck_handle(tim_buck_handle),
          tim_buck_channel(tim_buck_channel),
          tim_boost_handle(tim_boost_handle),
          tim_boost_channel(tim_boost_channel),
          gpio_port_output_ctl(gpio_port_output_ctl),
          gpio_pin_output_ctl(gpio_pin_output_ctl),
          pid_filter(.02f, .00f, 0.f, -1.f, 1.f),
          output_voltage_getter(output_voltage_getter) {}

    void init();
    void set_output_state(bool enable);
    void timer_handler();
    void set_target(float target);
};