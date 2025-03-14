#include "power_control.hpp"
#include "clamp.hpp"
#include <cmath>

void PowerControl::tim_sync_start() {
    __HAL_TIM_SetCompare(&tim_buck_handle, tim_buck_channel, __HAL_TIM_GetAutoreload(&tim_buck_handle) - 1);
    __HAL_TIM_SetCompare(&tim_boost_handle, tim_boost_channel, __HAL_TIM_GetAutoreload(&tim_boost_handle) - 1);
    HAL_TIM_PWM_Start(&tim_buck_handle, tim_buck_channel);
    HAL_TIM_PWM_Start(&tim_boost_handle, tim_boost_channel);
    HAL_TIMEx_PWMN_Start(&tim_buck_handle, tim_buck_channel);
    HAL_TIMEx_PWMN_Start(&tim_boost_handle, tim_boost_channel);

    __HAL_TIM_MOE_ENABLE(&tim_buck_handle);
    __HAL_TIM_ENABLE(&tim_buck_handle);
    HAL_TIM_Base_Start(&tim_buck_handle);
}

void PowerControl::tim_stop() {
    HAL_TIM_PWM_Stop(&tim_buck_handle, tim_buck_channel);
    HAL_TIM_PWM_Stop(&tim_boost_handle, tim_boost_channel);
    HAL_TIM_Base_Stop(&tim_boost_handle);
    HAL_TIM_Base_Stop(&tim_buck_handle);
}

void PowerControl::set_output_value(float output) {
    output = clamp(-1.f, output, 1.f);
    if (output < 0.f) {
        __HAL_TIM_SetCompare(&tim_buck_handle, tim_buck_channel, (__HAL_TIM_GetAutoreload(&tim_buck_handle) - 1) * (1.f - std::abs(output)));
        __HAL_TIM_SetCompare(&tim_boost_handle, tim_boost_channel, __HAL_TIM_GetAutoreload(&tim_boost_handle) - 1);
    } else {
        __HAL_TIM_SetCompare(&tim_buck_handle, tim_buck_channel, __HAL_TIM_GetAutoreload(&tim_buck_handle) - 1);
        __HAL_TIM_SetCompare(&tim_boost_handle, tim_boost_channel, (__HAL_TIM_GetAutoreload(&tim_boost_handle) - 1) * (1.f - std::abs(output)));
    }
}

void PowerControl::init() {
    tim_sync_start();
    set_output_state(true);
    set_target(5);
}

void PowerControl::set_output_state(bool enable) {
    HAL_GPIO_WritePin(gpio_port_output_ctl, gpio_pin_output_ctl, enable ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
}

void PowerControl::timer_handler() {
    set_output_value(output_voltage_getter() >> pid_filter);
}

void PowerControl::set_target(float target) {
    pid_filter.set_target(target);
}
