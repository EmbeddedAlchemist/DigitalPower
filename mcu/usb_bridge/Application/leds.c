#include "leds.h"
#include "STC8H.H"

#include "config.h"

static uint8_t brightness;
static uint8_t leds_brightness[LEDS_COUNT]              = {0, 0, 0};
static bool leds_state[LEDS_COUNT]                     = {false, false, false};
static bool leds_blink_state[LEDS_COUNT]               = {false, false, false};
static uint16_t leds_blink_delay[LEDS_COUNT]           = {500, 500, 500};
static uint16_t leds_blink_delay_remaining[LEDS_COUNT] = {0, 0, 0};

void leds_init() {
    P3M0 |= 0x38;
    P3M1 &= ~0x38;
}

void leds_set_state(uint8_t index, bool is_on) {
    leds_state[index] = is_on;
}

void leds_set_brightness(uint8_t b) {
    brightness = b;
}

void leds_set_blink_state(uint8_t index, bool is_on) {
    leds_blink_state[index] = is_on;
}

void leds_set_blink_cycle(uint8_t index, uint16_t cycle) {
    leds_blink_delay[index] = cycle / 2;
    leds_blink_delay_remaining[index] = 0;
}

void leds_software_pwm_interrupt_handler(void) {
    static uint8_t counter = 0;
    counter++;

    if (counter >= LEDS_MAX_BRIGHTNESS)
        counter = 0;
    P33 = leds_state[0] && counter < leds_brightness[0];
    P34 = leds_state[1] && counter < leds_brightness[1];
    P35 = leds_state[2] && counter < leds_brightness[2];
}

void leds_blink_interrupt_handler(void) {
    uint8_t i;
    static uint8_t count = 0;
    if (count < (uint8_t)(1 / SYS_TIMER_CYCLE_MS)){
        count++;
        return;
    }
    count = 0;
    for (i = 0; i < LEDS_COUNT; i++) {
        if (leds_blink_state[i] == false) {
            leds_brightness[i] = leds_state[i] ? brightness : 0;
        } else {
            if(leds_blink_delay_remaining[i] == 0){
                leds_blink_delay_remaining[i] = leds_blink_delay[i];
                leds_brightness[i] = leds_brightness[i] ? 0 : brightness;
            }
            leds_blink_delay_remaining[i]--;
        }
    }
}