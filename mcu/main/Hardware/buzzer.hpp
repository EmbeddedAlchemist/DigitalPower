#include "hal_header.hpp"
#include <cstdint>
#include <cstdlib>
#include <initializer_list>

using BuzzerAmpFunctionPtr = std::uint32_t (*)(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total);

std::uint32_t buzzer_amp_function_linear_up(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total);
std::uint32_t buzzer_amp_function_linear_down(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total);
std::uint32_t buzzer_amp_function_max(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total);

struct BuzzerNote {
    std::uint16_t freq;
    std::uint16_t duration_ms;
    float amp;
    BuzzerAmpFunctionPtr amp_function;

    inline constexpr BuzzerNote(
        std::uint16_t freq,
        std::uint16_t duration_ms,
        float amp                         = 1.f,
        BuzzerAmpFunctionPtr amp_function = buzzer_amp_function_linear_down)
        : freq(freq),
          duration_ms(duration_ms),
          amp(amp),
          amp_function(amp_function) {}
};

struct BuzzerNoteSequence {
    std::size_t length;
    const BuzzerNote *sequence;
};

#define DEFINE_BUZZER_NOTE_SEQUENCE(name, ...)                                    \
    static const BuzzerNote __macro_generated_##name##_storage[] = {__VA_ARGS__}; \
    const BuzzerNoteSequence name                                = {sizeof(__macro_generated_##name##_storage) / sizeof(__macro_generated_##name##_storage[0]), __macro_generated_##name##_storage};

class Buzzer {
  private:
    struct PlayingState {
        const BuzzerNote *sequence;
        std::size_t sequence_length;
        std::size_t current_position;

        std::uint32_t tick_total;
        std::uint32_t tick_remain;
        std::uint32_t max_compare;
        bool cycle_end;
    };

  private:
    TIM_HandleTypeDef &tim_amp_handler;
    TIM_HandleTypeDef &tim_freq_handler;
    const std::uint32_t tim_amp_channel;
    // const std::uint32_t tim_freq_channel;
    PlayingState current_state;
    float volume;

  private:
    void playEmptyNote(std::uint32_t duration_ms);
    void playNote(const BuzzerNote &note);
    std::uint32_t calc_amp_compare(const BuzzerNote &note, std::uint32_t max_compare, std::uint32_t tick_total, std::uint32_t tick_remain);
    const BuzzerNote &get_current_note();

    void freq_cycle_half();
    void freq_cycle_end();

  public:
    inline constexpr Buzzer(
        TIM_HandleTypeDef &tim_amp_handler,
        std::uint32_t tim_amp_channel,
        TIM_HandleTypeDef &tim_freq_handler)
        : tim_amp_handler(tim_amp_handler),
          tim_freq_handler(tim_freq_handler),
          tim_amp_channel(tim_amp_channel),
          current_state{},
          volume(1.f) {}

    void play(const BuzzerNoteSequence &seq);
    void stop();
    void set_volume(float volume);
    void on_tim_freq_period_elapsed(void);
};
