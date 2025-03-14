#pragma once

#include "button.hpp"
#include "buzzer.hpp"
#include "console.hpp"
#include "encoder.hpp"
#include "lcd.hpp"
#include "power_sample.hpp"
#include "serial_led.hpp"
#include "tmp102.hpp"
#include "usbpd.hpp"
#include "power_control.hpp"

extern SerialLed<9> serial_led;
extern Buzzer buzzer;
extern Lcd lcd;
extern Console console;
extern PowerSample power_sample;
extern TMP102 temperature_sensor_mosfet;
extern USBPD pd;
extern Button button1, button2, button_enc;
extern Encoder encoder;
extern PowerControl power_control;

void onboard_resource_init_after_os_ready();
