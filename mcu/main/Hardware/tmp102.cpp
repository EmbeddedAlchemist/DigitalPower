#include "tmp102.hpp"
#include "onboard_resource.hpp"

void TMP102::update() {
    std::uint16_t raw_tempurature = 0;
    HAL_I2C_Mem_Read_IT(&handle, addr, 0x0, I2C_MEMADD_SIZE_8BIT, reinterpret_cast<std::uint8_t *>(&raw_tempurature), sizeof(raw_tempurature));
}

float TMP102::get_temperature() {
    return temperature;
}

void TMP102::state_machine_loop_handle() {
    switch (state) {
    case UpdateState::waiting_for_next_update:
        if (HAL_GetTick() - last_update_time > 250) {
            last_update_time = HAL_GetTick();
            state            = UpdateState::send_read_request;
        }
        break;

    case UpdateState::send_read_request:
        read_write_buffer[0]      = 0x00;
        last_operation_start_time = HAL_GetTick();
        last_transfer_result      = ReadWriteResult::waiting;
        HAL_I2C_Master_Transmit_IT(&handle, addr, read_write_buffer, 1);
        state = UpdateState::waiting_for_send_end;
        break;

    case UpdateState::waiting_for_send_end:
        switch (last_transfer_result) {
        case ReadWriteResult::waiting:
            if (HAL_GetTick() - last_operation_start_time > 100) {
                state = UpdateState::waiting_for_next_update;
                console.error("TMP102 send read request timeout");
            }
            break;
        case ReadWriteResult::success:
            state                     = UpdateState::waiting_for_data_ready;
            last_operation_start_time = HAL_GetTick();
            break;
        case ReadWriteResult::failed:
            console.error("TMP102 send read request failed");
            state = UpdateState::waiting_for_next_update;
            break;
        }
        break;

    case UpdateState::waiting_for_data_ready:
        if (HAL_GetTick() - last_operation_start_time > 10)
            state = UpdateState::read_data;
        break;

    case UpdateState::read_data:
        read_write_buffer[0]      = 0x00;
        last_operation_start_time = HAL_GetTick();
        last_transfer_result      = ReadWriteResult::waiting;
        HAL_I2C_Master_Receive_IT(&handle, addr, read_write_buffer, 2);
        state = UpdateState::waiting_for_read_end;
        break;

    case UpdateState::waiting_for_read_end:
        std::int16_t data = 0;
        switch (last_transfer_result) {
        case ReadWriteResult::waiting:
            if (HAL_GetTick() - last_operation_start_time > 100) {
                state = UpdateState::waiting_for_next_update;
                console.error("TMP102 read timeout");
            }
            break;
        case ReadWriteResult::success:
            state = UpdateState::waiting_for_next_update;
            data  = ((static_cast<std::uint16_t>(read_write_buffer[0]) << 8) | static_cast<std::uint16_t>(read_write_buffer[1]));
            data /= 16;
            temperature = 0.0625f * data;
            break;
        case ReadWriteResult::failed:
            console.error("TMP102 read failed");
            state = UpdateState::waiting_for_next_update;
            break;
        }
        break;
    }
}

void TMP102::on_read_write_success() {
    last_transfer_result = ReadWriteResult::success;
}

void TMP102::on_read_write_failure() {
    last_transfer_result = ReadWriteResult::failed;
}
