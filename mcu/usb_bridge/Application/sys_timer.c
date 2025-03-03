#include <STC8H.H>

#include "sys_timer.h"
#include "leds.h"
#include "bit_op.h"

#include "config.h"

static volatile uint32_t millis_counter = 0;

void sys_timer_init(void) {
	T4T3M |= 0x20;			//定时器时钟1T模式
	T4L = 0x80;				//设置定时初始值
	T4H = 0xDA;				//设置定时初始值
	T4T3M |= 0x80;			//定时器4开始计时
    set_bit(IE2, 6);
    
}

uint32_t millis(void) {
    return millis_counter;
}

static uint32_t millis_counter_increase(void) {
    static uint8_t counter = 0;
    if (counter < 5)
        counter++;
    else
        counter = 0, millis_counter++;
}

static void sys_timer_interrupt_handler(void) interrupt TMR4_VECTOR {
    // clear_bit(AUXINTIF, 2);
    millis_counter_increase();
    leds_software_pwm_interrupt_handler();
    leds_blink_interrupt_handler();
}
