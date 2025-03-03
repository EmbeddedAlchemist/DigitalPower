#include "lcd.hpp"
#include "clamp.hpp"

void Lcd::prepare_to_write_8bit_data() {
    HAL_GPIO_WritePin(io_port_dc, io_pin_dc, GPIO_PinState::GPIO_PIN_SET);
    LL_SPI_SetDataWidth(spi_handle.Instance, LL_SPI_DATAWIDTH_8BIT);
}

void Lcd::prepare_to_write_16bit_data() {
    HAL_GPIO_WritePin(io_port_dc, io_pin_dc, GPIO_PinState::GPIO_PIN_SET);
    LL_SPI_SetDataWidth(spi_handle.Instance, LL_SPI_DATAWIDTH_16BIT);
}

void Lcd::prepare_to_write_reg() {
    HAL_GPIO_WritePin(io_port_dc, io_pin_dc, GPIO_PinState::GPIO_PIN_RESET);
    LL_SPI_SetDataWidth(spi_handle.Instance, LL_SPI_DATAWIDTH_8BIT);
}

bool Lcd::wait_for_spi_transmit(std::uint32_t timeout_ms) {
    auto result = osThreadFlagsWait(signal_spi_transmit_fail | signal_spi_transmit_success, osFlagsWaitAny, timeout_ms);
    if (!(result & osFlagsError) && result & signal_spi_transmit_success)
        return true;
    return false;
}

bool Lcd::write_reg(std::uint8_t reg) {
    current_thread = osThreadGetId();
    prepare_to_write_reg();
    HAL_SPI_Transmit_DMA(&spi_handle, &reg, sizeof(reg));
    return wait_for_spi_transmit();
}

bool Lcd::write_8bit_data(const std::uint8_t *data, std::size_t length) {
    current_thread = osThreadGetId();
    prepare_to_write_8bit_data();
    HAL_SPI_Transmit_DMA(&spi_handle, const_cast<std::uint8_t *>(data), length);
    return wait_for_spi_transmit();
}

bool Lcd::write_16bit_data(const std::uint16_t *data, std::size_t length) {
    current_thread = osThreadGetId();
    prepare_to_write_16bit_data();
    HAL_SPI_Transmit_DMA(&spi_handle, const_cast<std::uint8_t *>(reinterpret_cast<const std::uint8_t *>(data)), length);
    return wait_for_spi_transmit();
}

bool Lcd::write_command(std::uint8_t command, const std::uint8_t *param, std::size_t length) {
    if (write_reg(command) == false)
        return false;
    if (param == nullptr)
        return true;
    if (write_8bit_data(param, length) == false)
        return false;
    return true;
}

bool Lcd::write_command(std::uint8_t command, std::initializer_list<std::uint8_t> param) {
    return write_command(command, param.begin(), param.size());
}

bool Lcd::write_command(std::uint8_t command) {
    return write_command(command, nullptr, 0);
}

void Lcd::init() {
    set_brightness(brightness);
    HAL_TIM_OC_Start(&tim_bg_light_handle, tim_bg_light_channel);

    write_command(0x01);                                       // reset;
    osDelay(5);                                                // wait for reset
    write_command(0x36, {0x00});                               // Memory data access
    write_command(0x3a, {0b1010101});                          // Color Mode: 65K 16bit
    write_command(0xb2, {0x0c, 0x0c, 0x00, 0x33, 0x33});       // Porch: BPA, FPA, PSEN, BPB, BPC
    write_command(0xb7, {0x35});                               // Gate: VGHS=13.26V, VGLS= -10.43V
    write_command(0xbb, {0x19});                               // VCOM: 0.725V
    write_command(0xc0, {0b0101100});                          // LCM: XMY=0, XBGR=1, XINV=0, XMX=1, XMH=1, XMV=0, XGS=0
    write_command(0xc2, {0x01, 0xFF});                         // VDV&VRH: CMDEN=1
    write_command(0xc3, {0x12});                               // VRH: VAP=4.45+...V VAN=-4.45+...V
    write_command(0xc4, {0x20});                               // VDV: VDV=0V
    write_command(0xc6, {0x0f});                               // Frame Rate: 60Hz
    write_command(0xd0, {0xa4, 0xa1});                         // Power: AVDD=Reserved, AVCL=-5.0V VDS=2.3V
    write_command(0xe0,                                        // Positive Voltage Gamma
                  {0xd0, 0x04, 0xd0, 0x11, 0x13, 0x2b, 0x3f,   //
                   0x54, 0x4c, 0x18, 0x0d, 0x0b, 0x1f, 0x23}); //
    write_command(0xe1,                                        // Negative Voltage Gamma
                  {0xd0, 0x04, 0x0c, 0x11, 0x13, 0x2c, 0x3f,   //
                   0x44, 0x51, 0x2f, 0x15, 0x1f, 0x20, 0x23}); //
    write_command(0x21);                                       // Display Inversion ON
    write_command(0x11);                                       // Sleep out
    osDelay(120);                                              // wait for sleep out
    write_command(0x29);                                       // display on
}

void Lcd::set_window(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end) {
    std::uint16_t
        x_start_offseted = x_start + 52,
        x_end_offseted   = x_end + 52,
        y_start_offseted = y_start + 40,
        y_end_offseted   = y_end + 40;

    write_command(
        0x2a,
        {
            static_cast<std::uint8_t>(x_start_offseted >> 8 & 0xff),
            static_cast<std::uint8_t>(x_start_offseted & 0xff),
            static_cast<std::uint8_t>(x_end_offseted >> 8 & 0xff),
            static_cast<std::uint8_t>(x_end_offseted & 0xff),
        });
    write_command(
        0x2b,
        {
            static_cast<std::uint8_t>(y_start_offseted >> 8 & 0xff),
            static_cast<std::uint8_t>(y_start_offseted & 0xff),
            static_cast<std::uint8_t>(y_end_offseted >> 8 & 0xff),
            static_cast<std::uint8_t>(y_end_offseted & 0xff),
        });
    write_command(0x2c);
}

void Lcd::set_pixel(std::uint16_t x, std::uint16_t y, std::uint16_t color) {
    set_window(x, y, x, y);
    write_8bit_data((uint8_t *)&color, sizeof(color));
}

void Lcd::update_area(exgraphics::Offset offset_start, exgraphics::Size size, const ColorType *buffer) {
    exgraphics::Offset offset_end = offset_start + size.to_offset();
    set_window(offset_start.x, offset_start.y, offset_end.x, offset_end.y);
    write_16bit_data(reinterpret_cast<const uint16_t *>(buffer), size.get_area() * sizeof(ColorType));
}

void Lcd::set_brightness(float b) {
    brightness       = clamp(0.f, b, 1.f);
    auto auto_reload = __HAL_TIM_GetAutoreload(&tim_bg_light_handle) * brightness;
    __HAL_TIM_SetCompare(&tim_bg_light_handle, tim_bg_light_channel, auto_reload);
}

void Lcd::on_spi_transmit_success() {
    osThreadFlagsSet(current_thread, signal_spi_transmit_success);
}

void Lcd::on_spi_transmit_fail() {
    osThreadFlagsSet(current_thread, signal_spi_transmit_fail);
}
