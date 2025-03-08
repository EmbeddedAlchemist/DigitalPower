#include "leds.h"
#include "sys_timer.h"
#include "isp_btn.h"
#include <STC8H.H>

void main(void) {
    EA = 1;
    isp_btn_init();
    leds_init();
    sys_timer_init();

    leds_set_brightness(16);
    leds_set_state(0, true);
    leds_set_state(1, true);
    leds_set_state(2, true);
    leds_set_blink_state(0, true);
    leds_set_blink_state(1, true);
    leds_set_blink_cycle(0, 1000);
    leds_set_blink_cycle(1, 1000);
    while(1){}

}
