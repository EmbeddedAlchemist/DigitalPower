#include "onboard_resource.hpp"

extern "C" void power_control_timer_handle(void *unused) {
    power_sample.handle_auto_gain();
    power_control.timer_handler();
    // console.debug("dc voltage:  %.2fV", power_sample.get_dc_voltage());
    // console.debug("pd voltage:  %.2fV", power_sample.get_pd_voltage());
    // console.debug("in voltage:  %.2fV", power_sample.get_in_voltage());
    // console.debug("out voltage: %.2fV", power_sample.get_out_voltage());
    // console.debug("in current:  %.2fA", power_sample.get_in_current());
    // console.debug("out current: %.2fA\n", power_sample.get_out_current());
}