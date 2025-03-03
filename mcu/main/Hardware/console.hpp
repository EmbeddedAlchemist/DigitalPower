#pragma once

#include "cmsis_os.h"
#include "hal_header.hpp"
#include <cstdarg>

class Console {
  public:
    enum class LogLevel {
      none = 0,
      error,
      warning,
      info,
      debug,
      verbose
    };
  private:
    static constexpr std::uint32_t signal_transmit_success = 1ul << 0;
    static constexpr std::uint32_t signal_transmit_fail    = 1ul << 0;

    union {
        char storage[128] = {};
        struct {
            char level;
            char timestamp[10];
            char space;
            char content[116];
        };
    } buffer;

    UART_HandleTypeDef &handle;
    osMessageQId buffer_queue = nullptr;
    osThreadId current_thread = nullptr;
    LogLevel log_level            = LogLevel::verbose;

    bool wait_for_transit_finish();

    int write_log(std::uint32_t timestamp, char log_level, const char *log_format, std::va_list args);

  public:
    inline constexpr Console(UART_HandleTypeDef &handle)
        : handle(handle) {};

    int error(const char *format, ...);
    int warning(const char *format, ...);
    int debug(const char *format, ...);
    int info(const char *format, ...);
    int verbose(const char *format, ...);

    int arg_log(char log_level, const char *log_format, std::va_list args);

    inline void init(osMessageQId q) { buffer_queue = q; }

    void on_transmit_finish();
    void on_transmit_fail();

    void handle_send();
};

extern Console console;