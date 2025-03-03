#pragma once

// #include <algorithm>

#include "hal_header.hpp"

#include <cstddef>

class UsbBridge {

  private:
    static constexpr std::uint32_t package_header = 0xA5;
    static constexpr std::size_t data_frame_size  = 64;

    enum class PackageType : std::uint8_t {
        heartbeat,
        ack,
        data,

        max
    };

    struct Package {
        std::uint8_t header[4];
        std::uint8_t length;
        std::uint8_t id;
        PackageType type;
        std::uint8_t checksum;
    };

    struct PackageHeartbeat : public Package {
    };

    enum class AckType : std::uint8_t {
        ok,
        checksum_error,
    };

    struct PackageAck : public Package {
        std::uint8_t for_id;
        AckType ack_type;
    };

    struct PackageData : public Package {
        std::uint8_t data[data_frame_size];
    };

    struct Buffer {
        union {
            Package package;
            PackageHeartbeat heartbeat;
            PackageAck ack;
            PackageData data;

            std::uint8_t buffer[sizeof(PackageData)];
        };
    };

    struct ReceiveBuffer : public Buffer {
        std::uint8_t *write_ptr;

        void reset();
        /**
         * @return true done
         */
        bool receive(std::uint8_t data);
        bool verify();
    };

    struct SendBuffer : public Buffer {
    };

  private:
    ReceiveBuffer recv_buffer[2];
    ReceiveBuffer *curr_recv_buffer;

    SendBuffer send_buffer;
    SendBuffer ack_buffer;
    USART_HandleTypeDef &handle;

    

  public:
    void communication_reset();
    void on_receive_data(std::uint8_t data);
    void on_transmit_finish();
};