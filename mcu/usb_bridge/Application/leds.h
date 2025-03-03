#ifndef _leds_h_
#define _leds_h_

#include "numeric_type.h"



void leds_init();
void leds_set_state(uint8_t index, bool is_on);
void leds_set_brightness(uint8_t brightness);

void leds_set_blink_state(uint8_t index, bool is_on);
void leds_set_blink_cycle(uint8_t index, uint16_t cycle);

void leds_software_pwm_interrupt_handler(void);
void leds_blink_interrupt_handler(void);

#endif