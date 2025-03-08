#include "hal_header.hpp"
#include "i2c.h"
#include "onboard_resource.hpp"
#include "spi.h"
#include "tim.h"
#include "usart.h"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3) {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3))
            encoder.on_decrease();
        else
            encoder.on_increase();
    }
    if (htim == &htim6)
        buzzer.on_tim_freq_period_elapsed();
    if (htim->Instance == TIM7)
        HAL_IncTick();
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi2)
        lcd.on_spi_transmit_success();
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi2)
        lcd.on_spi_transmit_fail();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3)
        console.on_transmit_finish();
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3)
        console.on_transmit_fail();
}

extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == &hi2c3)
        temperature_sensor_mosfet.on_read_write_success();
}

extern "C" void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == &hi2c3)
        temperature_sensor_mosfet.on_read_write_success();
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c == &hi2c3)
        temperature_sensor_mosfet.on_read_write_failure();
}
extern "C" void HAL_GPIO_EXTI_Callback(std::uint16_t gpio_pin) {
    if (gpio_pin == GPIO_PIN_7) {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PinState::GPIO_PIN_SET)
            button_enc.on_press();
        else
            button_enc.on_release();
    } else if (gpio_pin == GPIO_PIN_8) {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PinState::GPIO_PIN_SET)
            button1.on_press();
        else
            button1.on_release();
    } else if (gpio_pin == GPIO_PIN_9) {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PinState::GPIO_PIN_SET)
            button2.on_press();
        else
            button2.on_release();
    }
}
