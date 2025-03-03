#include "hal_header.hpp"
#include <clamp.hpp>
#include <cstddef>
#include <cstdint>

template <std::uint32_t tp_led_count>
class SerialLed {
  public:
    constexpr static std::size_t led_count = tp_led_count;

  private:
    struct LedPixelData {
        std::uint16_t buffer[3][8];
    };

    struct Color {
        std::uint8_t grb[3];
        Color() = default;
        inline constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
            : grb{g, r, b} {}
        inline void set(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
            grb[0] = g;
            grb[1] = r;
            grb[2] = b;
        }
    };

    TIM_HandleTypeDef &tim_handle;
    std::uint32_t tim_channel;
    LedPixelData dma_buffer[led_count];
    Color color[led_count];

    std::uint16_t
        bit_0_compare,
        bit_1_compare;

    float brightness;

  public:
    inline SerialLed(TIM_HandleTypeDef &tim_handle, std::uint32_t tim_channel)
        : tim_handle(tim_handle), tim_channel(tim_channel), brightness(1.f) {}

    /**
     * @brief must call after timer init
     *
     */
    inline void init() {
        std::uint32_t auto_reload_value = __HAL_TIM_GetAutoreload(&tim_handle) + 1;
        bit_0_compare                   = auto_reload_value / 3 * 1;
        bit_1_compare                   = auto_reload_value / 3 * 2;

        for (int i = 0; i < led_count; i++)
            set_pixel(i, 0, 0, 0);

        update();
    }

    inline void update() {
        for (std::size_t index = 0; index < led_count; index++) {
            for (std::uint32_t i = 0; i < sizeof(color[0]); i++) {
                Color computed_color(
                    color[index].grb[1] * brightness,
                    color[index].grb[0] * brightness,
                    color[index].grb[2] * brightness);

                for (std::uint32_t j = 0; j < 8; j++) {
                    dma_buffer[index].buffer[i][j] = ((computed_color.grb[i] & (0x80 >> j)) == 0) ? bit_0_compare : bit_1_compare;
                }
            }
        }
        HAL_TIM_OC_Start_DMA(
            &tim_handle,
            tim_channel,
            reinterpret_cast<std::uint32_t *>(dma_buffer),
            sizeof(LedPixelData) * led_count);
    }

    inline void set_pixel(std::size_t index, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        color[index].set(r, g, b);
    }

    void set_brightness(float brightness) {
        brightness = clamp(0.f, brightness, 1.f);
    }
};