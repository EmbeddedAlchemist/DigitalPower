#pragma once

#include <cstdint>
#include <functional>

class Timeout {
  private:
    std::uint32_t delay_tick = 0;
    std::uint32_t start_tick = 0;
    std::function<void(void)> callback;
    bool running = false;

  public:
    inline Timeout(std::uint32_t delay_tick = 0)
        : delay_tick(delay_tick) {}
    void register_callback(std::function<void(void)> callback);
    void start();
    void end();
    void set_delay(std::uint32_t delay_tick);
    void loop_handler();
    bool is_running();

    float get_progress();
};