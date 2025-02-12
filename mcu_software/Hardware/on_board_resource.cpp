#include "tim.h"

#include "serial_led.hpp"
#include "buzzer.hpp"

SerialLed<7> serial_led(htim2, TIM_CHANNEL_2);
Buzzer buzzer(htim17, TIM_CHANNEL_1, htim6);