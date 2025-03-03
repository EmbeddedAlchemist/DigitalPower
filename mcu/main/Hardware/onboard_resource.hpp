#pragma once

#include "serial_led.hpp"
#include "buzzer.hpp"
#include "lcd.hpp"
#include "console.hpp"
#include "power_sample.hpp"
#include "tmp102.hpp"
#include "usbpd.hpp"

extern SerialLed<9> serial_led;
extern Buzzer buzzer;
extern Lcd lcd;
extern Console console;
extern PowerSample power_sample;
extern TMP102 temperature_sensor_mosfet;
extern USBPD pd;

void onboard_resource_init_after_os_ready();
