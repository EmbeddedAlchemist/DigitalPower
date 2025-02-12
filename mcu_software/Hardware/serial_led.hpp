#include "hal_header.hpp"
#include <cstdint>
#include <cstdlib>

template <std::size_t tp_led_count>
class SerialLed {
  public:
    constexpr static std::size_t led_count = tp_led_count;

  private:
    struct LedPixelData {
        std::uint16_t buffer[3][8];
    };

    TIM_HandleTypeDef &tim_handle;
    std::uint32_t tim_channel;
    LedPixelData dma_buffer[led_count];

    std::uint16_t
        bit_0_compare,
        bit_1_compare;

  public:
    SerialLed(TIM_HandleTypeDef &tim_handle, std::uint32_t tim_channel)
        : tim_handle(tim_handle), tim_channel(tim_channel){}

    /**
     * @brief must call after timer init
     *
     */
    void init() {
        std::uint32_t auto_reload_value = __HAL_TIM_GetAutoreload(&tim_handle);
        bit_0_compare = auto_reload_value / 3 * 1;
        bit_1_compare = auto_reload_value / 3 * 2;
    }

    void update() {
        HAL_TIM_OC_Start_DMA(
            &tim_handle,
            tim_channel,
            reinterpret_cast<std::uint32_t *>(dma_buffer),
            sizeof(dma_buffer) / sizeof(std::uint16_t));
    }

    void set_pixel(std::size_t index, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        std::uint8_t color[] = {g, r, b};
        for (std::uint32_t i = 0; i < sizeof(color); i++)
            for(std::uint32_t j = 0; j < 8; j++)
                dma_buffer[index].buffer[i][j] = ((color[i] & (0x80 >> j)) == 0) ? bit_0_compare : bit_1_compare;
    }
};