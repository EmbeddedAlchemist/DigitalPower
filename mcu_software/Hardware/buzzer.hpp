#include "hal_header.hpp"
#include <cstdint>
#include <cstdlib>
#include <initializer_list>

using BuzzerAmpFunction = std::uint32_t (*)(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total);

struct BuzzerNote {
    std::uint16_t freq;
    std::uint16_t duration_ms;
    float amp;
    BuzzerAmpFunction amp_function;

    constexpr BuzzerNote(
        std::uint16_t freq,
        std::uint16_t duration_ms,
        float amp,
        BuzzerAmpFunction amp_function)
        : freq(freq),
          duration_ms(duration_ms),
          amp(amp),
          amp_function(amp_function) {}
};

struct BuzzerNoteSequenceUniform {
    std::size_t length;
    const BuzzerNote *note_array;
};

template <std::size_t tp_length>
struct BuzzerNoteSequence  {

};

class Buzzer {
  private:
    TIM_HandleTypeDef &tim_amp_handler;
    TIM_HandleTypeDef &tim_freq_handler;
    std::uint32_t tim_amp_channel;

  public:
    constexpr Buzzer(
        TIM_HandleTypeDef &tim_amp_handler,
        std::uint32_t tim_amp_channel,
        TIM_HandleTypeDef &tim_freq_handler)
        : tim_amp_handler(tim_amp_handler),
          tim_freq_handler(tim_freq_handler),
          tim_amp_channel(tim_amp_channel){}

    void play();

    void on_tim_freq_update();
};