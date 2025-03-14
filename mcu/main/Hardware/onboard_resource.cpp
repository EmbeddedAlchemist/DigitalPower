#include "onboard_resource.hpp"
#include "adc.h"
#include "i2c.h"
#include "main.h"
#include "opamp.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

extern osMessageQueueId_t consoleBufferQueueHandle;

SerialLed<9> serial_led(htim2, TIM_CHANNEL_3);
Buzzer buzzer(htim17, TIM_CHANNEL_1, htim6);
Lcd lcd(hspi2, htim15, TIM_CHANNEL_1, lcd_dc_GPIO_Port, lcd_dc_Pin);
Console console(huart3);
PowerSample power_sample(hadc1, hadc2, hopamp2, hopamp3);
TMP102 temperature_sensor_mosfet(hi2c3, 0b10010000);
USBPD pd(0);
Button button1, button2, button_enc;
Encoder encoder(htim3);
PowerControl power_control(
    htim1, TIM_CHANNEL_1,
    htim8, TIM_CHANNEL_1,
    output_ctrl_GPIO_Port, output_ctrl_Pin,
    []() { return power_sample.get_out_voltage(); });

void onboard_resource_init_after_os_ready() {
    console.init(consoleBufferQueueHandle);
    power_sample.init();
    encoder.init();
    power_control.init();
}