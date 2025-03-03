#pragma once

#include "hal_header.hpp"
#include <cstdint>

class TMP102 {
  private:
    enum class UpdateState {
        waiting_for_next_update,
        send_read_request,
        waiting_for_send_end,
        waiting_for_data_ready,
        read_data,
        waiting_for_read_end
    };

    enum class ReadWriteResult {
        waiting,
        success,
        failed
    };

    I2C_HandleTypeDef &handle;
    std::uint8_t addr;
    float temperature                    = 0.f;
    UpdateState state                    = UpdateState::waiting_for_next_update;
    std::uint32_t last_operation_start_time   = 0;
    std::uint32_t last_update_time            = 0;
    ReadWriteResult last_transfer_result = ReadWriteResult::waiting;

    std::uint8_t read_write_buffer[2] = {};

  public:
    inline constexpr TMP102(I2C_HandleTypeDef &handle, std::uint8_t addr_7bit_left_aligned)
        : handle(handle), addr(addr_7bit_left_aligned) {}

    void update();
    float get_temperature();

    void update_loop_handle();

    void on_read_write_success();
    void on_read_write_failure();
};