#include "gui_thread.hpp"
#include "onboard_resource.hpp"
#include "usb_thread.hpp"
#include "tim.h"

DEFINE_BUZZER_NOTE_SEQUENCE(buzzer_seq_btn_click, BuzzerNote(4000, 200));
DEFINE_BUZZER_NOTE_SEQUENCE(buzzer_seq_operation_deny, BuzzerNote(2000, 100), BuzzerNote(2000, 200));
DEFINE_BUZZER_NOTE_SEQUENCE(buzzer_seq_start_up, BuzzerNote(3000, 120), BuzzerNote(3000, 800));

extern osThreadId guiThreadHandle;
extern osThreadId usbThreadHandle;
extern osTimerId powerControlTimerHandle;

extern "C" void user_main(void *unused_arg) {
    onboard_resource_init_after_os_ready();
    console.info("user main start");
    console.info("System Clock Freq: %f", SystemCoreClock / 1000000.f);
    std::uint32_t retval = osThreadFlagsSet(usbThreadHandle, usb_thread_signal_ready);
    console.verbose("Send usb_thread_signal_ready to usb_thread, result is %u", retval);
    osStatus_t status = osTimerStart(powerControlTimerHandle, 50);
    console.verbose("Start power_control_timer, %s with code %d.", status == osOK ? "succeed" : "failed", status);

    serial_led.init();
//    serial_led.set_pixel(0, 16, 16, 16);
//    serial_led.set_pixel(1, 16, 16, 16);
    serial_led.set_pixel(2, 16, 16, 16);
    serial_led.set_pixel(3, 16, 16, 16);
    serial_led.set_pixel(4, 16, 16, 16);
    serial_led.set_pixel(5, 0, 255, 255);
    serial_led.set_pixel(6, 255, 64, 0);
    serial_led.set_pixel(7, 255, 64, 0);
    serial_led.set_pixel(8, 0, 255, 0);
    serial_led.set_brightness(1);
    serial_led.update();

    pd.set_voltage(20.f);

    buzzer.set_volume(0.2f);
    buzzer.play(buzzer_seq_start_up);
    lcd.init();
    console.verbose("Send gui_thread_signal_display_device_ready to gui_thread, result is %u", osThreadFlagsSet(guiThreadHandle, gui_thread_signal_display_device_ready));
    console.debug("TIM1 reload: 0x%x", TIM1->ARR);

    while (true) {

        console.debug("PD:%.2f DC:%.2f IN:%.2f OUT:%.2f", power_sample.get_pd_voltage(), power_sample.get_dc_voltage(), power_sample.get_in_voltage(), power_sample.get_out_voltage());
        temperature_sensor_mosfet.state_machine_loop_handle();
        button1.loop_handle();
        button2.loop_handle();
        button_enc.loop_handle();
        console.handle_send();

        osThreadYield();
    }
}

extern "C" void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
    asm("BKPT 0");
    while (true) {
    }
}
