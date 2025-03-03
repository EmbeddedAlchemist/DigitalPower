#include "hal_header.hpp"
#include <cstddef>
#include <cstdint>

#include <initializer_list>

#include "cmsis_os.h"

#include "RGB565.hpp"
#include "display_adapter.hpp"

class Lcd : public exgraphics::DisplayAdapter<RGB565> {
  private:
    static constexpr std::uint32_t signal_spi_transmit_success = 1ul << 0;
    static constexpr std::uint32_t signal_spi_transmit_fail    = 1ul << 0;

  private:
    SPI_HandleTypeDef &spi_handle;
    TIM_HandleTypeDef &tim_bg_light_handle;
    std::uint32_t tim_bg_light_channel;
    GPIO_TypeDef *io_port_dc;
    std::uint32_t io_pin_dc;

    osThreadId current_thread       = nullptr;
    std::uint16_t repeat_write_data = 0;
    float brightness                = 0.f;

  private:
    void prepare_to_write_8bit_data();
    void prepare_to_write_16bit_data();
    void prepare_to_write_reg();

    bool wait_for_spi_transmit(std::uint32_t timeout_ms = 500);

    bool write_reg(std::uint8_t reg);
    bool write_8bit_data(const std::uint8_t *data, std::size_t length);
    bool write_16bit_data(const std::uint16_t *data, std::size_t length);

  public:
    inline constexpr Lcd(
        SPI_HandleTypeDef &spi_handle,
        TIM_HandleTypeDef &tim_bg_light_handle,
        std::uint32_t tim_bg_light_channel,
        GPIO_TypeDef *io_port_dc,
        std::uint32_t io_pin_dc)
        : spi_handle(spi_handle),
          tim_bg_light_handle(tim_bg_light_handle),
          tim_bg_light_channel(tim_bg_light_channel),
          io_port_dc(io_port_dc),
          io_pin_dc(io_pin_dc) {}

    void init();

    bool write_command(std::uint8_t command, const std::uint8_t *param, std::size_t length);
    bool write_command(std::uint8_t command, std::initializer_list<std::uint8_t> param);
    bool write_command(std::uint8_t command);

    void set_window(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end);

    void set_pixel(std::uint16_t x, std::uint16_t y, std::uint16_t color);

    virtual void update_area(exgraphics::Offset offset, exgraphics::Size size, const ColorType *buffer) override final;

    void set_brightness(float brightness);

    void on_spi_transmit_success();
    void on_spi_transmit_fail();
};