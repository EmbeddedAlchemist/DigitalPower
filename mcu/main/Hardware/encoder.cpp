#include "encoder.hpp"

void Encoder::on_increase() {
    if (HAL_GetTick() - last_triggered_time <= debounce_delay_ms)
        return;
    last_triggered_time = HAL_GetTick();
    if (callback)
        callback(EventType::increase);
}

void Encoder::on_decrease() {
    if (HAL_GetTick() - last_triggered_time <= debounce_delay_ms)
        return;
    last_triggered_time = HAL_GetTick();
    if (callback)
        callback(EventType::decrease);
}

void Encoder::init() {
    // HAL_TIM_Base_Start_IT(&handler);
    __HAL_TIM_CLEAR_IT(&handler, TIM_IT_UPDATE);
    HAL_TIM_Encoder_Start(&handler, TIM_CHANNEL_ALL);
    __HAL_TIM_ENABLE_IT(&handler, TIM_IT_UPDATE);
}

void Encoder::register_callback(std::function<CallbackFunction> cb) {
    callback = cb;
}