#include "console.hpp"
#include <cstdio>

bool Console::wait_for_transit_finish() {
    auto result = osThreadFlagsWait(signal_transmit_success | signal_transmit_fail, osFlagsWaitAny, osWaitForever);
    if (!(result & osFlagsError) && result | signal_transmit_success)
        return true;
    return false;
}

int Console::write_log(std::uint32_t timestamp, char log_level, const char *log_format, std::va_list args) {
    if (buffer_queue == nullptr)
        return 0;
    buffer.level = log_level;
    snprintf(buffer.timestamp, sizeof(buffer.timestamp), "@%-9u", timestamp);
    buffer.space                = ' ';
    int content_len             = vsnprintf(buffer.content, sizeof(buffer.content), log_format, args);
    buffer.content[content_len] = '\n';
    if (content_len == sizeof(buffer.content)) {
        buffer.content[content_len - 1] = '.';
        buffer.content[content_len - 2] = '.';
        buffer.content[content_len - 3] = '.';
    }
    int len    = (buffer.content + content_len + 1) - buffer.storage;
    int result = 0;
    for (const char *ptr = buffer.storage; ptr < buffer.storage + len; ptr++, result++)
        if (osMessageQueuePut(buffer_queue, ptr, 0, 0) != osStatus_t::osOK)
            break;
    return result;
}

int Console::error(const char *format, ...) {
    if (log_level < LogLevel::error)
        return 0;
    std::va_list args;
    va_start(args, format);
    int res = write_log(HAL_GetTick(), 'E', format, args);
    va_end(args);
    return res;
}

int Console::warning(const char *format, ...) {
    if (log_level < LogLevel::warning)
        return 0;
    std::va_list args;
    va_start(args, format);
    int res = write_log(HAL_GetTick(), 'W', format, args);
    va_end(args);
    return res;
}

int Console::debug(const char *format, ...) {
    if (log_level < LogLevel::debug)
        return 0;
    std::va_list args;
    va_start(args, format);
    int res = write_log(HAL_GetTick(), 'D', format, args);
    va_end(args);
    return res;
}

int Console::info(const char *format, ...) {
    if (log_level < LogLevel::info)
        return 0;
    std::va_list args;
    va_start(args, format);
    int res = write_log(HAL_GetTick(), 'I', format, args);
    va_end(args);
    return res;
}

int Console::verbose(const char *format, ...) {
    if (log_level < LogLevel::verbose)
        return 0;
    std::va_list args;
    va_start(args, format);
    int res = write_log(HAL_GetTick(), 'V', format, args);
    va_end(args);
    return res;
}

int Console::arg_log(char log_level, const char *log_format, std::va_list args) {
    return write_log(HAL_GetTick(), log_level, log_format, args);
}

void Console::on_transmit_finish() {
    osThreadFlagsSet(current_thread, signal_transmit_success);
}

void Console::on_transmit_fail() {
    osThreadFlagsSet(current_thread, signal_transmit_fail);
}

void Console::handle_send() {
    char buf[64] = {};
    while (osMessageQueueGetCount(buffer_queue) > 0) {
        char *buf_end = buf;
        while (buf_end < buf + sizeof(buf) && osMessageQueueGet(buffer_queue, buf_end, nullptr, 0) == osOK) {
            buf_end++;
        }
        if (buf == buf_end) {
            continue;
        }
        current_thread = osThreadGetId();
        HAL_UART_Transmit_IT(&handle, reinterpret_cast<const std::uint8_t *>(buf), buf_end - buf);
        wait_for_transit_finish();
    }
}
