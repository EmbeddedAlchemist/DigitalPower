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

void onboard_resource_init_after_os_ready() {
    console.init(consoleBufferQueueHandle);
    power_sample.init();
}