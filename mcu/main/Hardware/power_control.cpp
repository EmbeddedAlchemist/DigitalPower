#include "power_control.hpp"

void PowerControl::tim_sync_start() {
    __HAL_TIM_SetCompare(&tim_buck_handle, tim_buck_channel, __HAL_TIM_GetAutoreload(&tim_buck_handle)  / 2);
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

void PowerControl::init() {
    tim_sync_start();
}
