#include "hal_header.hpp"
#include "i2c.h"
#include "onboard_resource.hpp"
#include "spi.h"
#include "tim.h"
#include "usart.h"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
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
