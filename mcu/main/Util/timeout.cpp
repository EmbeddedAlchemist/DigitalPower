#include "timeout.hpp"
#include "hal_header.hpp"
#include "clamp.hpp"

void Timeout::register_callback(std::function<void(void)> cb) {
    callback = cb;
}

void Timeout::start() {
    start_tick = HAL_GetTick();
    running = true;
}

void Timeout::end() {
    running = false;
}

void Timeout::set_delay(std::uint32_t d) {
    delay_tick = d;
}

void Timeout::loop_handler() {
    if(running == false)
        return;
    if(HAL_GetTick() >= start_tick + delay_tick){
        running = false;
        if(callback)
            callback();
    }

}

bool Timeout::is_running() {
    return running;
}

float Timeout::get_progress() {
    return clamp(0.f, (float)(HAL_GetTick() - start_tick) / delay_tick ,1.f);
}
