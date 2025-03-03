#include "usb_bridge.hpp"

void UsbBridge::on_receive_data(std::uint8_t data) {
    if (curr_recv_buffer == nullptr)
        return;
    if (curr_recv_buffer->receive(data) == true) {
        // handle package
    }
}

void UsbBridge::ReceiveBuffer::reset() {
    write_ptr = buffer;
}

bool UsbBridge::ReceiveBuffer::receive(std::uint8_t data) {

    if (write_ptr >= buffer && write_ptr < buffer + offsetof(Package, header) + sizeof(package.header)) {
        if (package_header != data)
            goto fail;
    } else if (write_ptr < buffer + offsetof(Package, length) + sizeof(package.length)) { // recv length
        if (data > sizeof(buffer) || data < sizeof(Package))
            goto fail;
    } else if (write_ptr < buffer + offsetof(Package, type) + sizeof(package.type)) {
        if (data >= static_cast<std::uint8_t>(PackageType::max))
            goto fail;
    }

    *(write_ptr++) = data;

    if (write_ptr >= buffer + sizeof(Package) && write_ptr == buffer + package.length) {
        if (verify() == false) {
            goto fail;
        }
        return true;
    }
    return false;

fail:
    reset();
    return false;
}

bool UsbBridge::ReceiveBuffer::verify() {
    std::uint8_t checksum = package.checksum;
    package.checksum      = 0;
    std::uint8_t sum      = 0;
    for (std::uint8_t *p = buffer; p < buffer + package.length; p++)
        sum += *p;
    bool result      = sum == checksum;
    package.checksum = checksum;
    return result;
}
