#include "buzzer.hpp"
#include "clamp.hpp"

std::uint32_t buzzer_amp_function_linear_up(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total) {
    return amp_max * (time_total - time_remain) / time_total;
}

std::uint32_t buzzer_amp_function_linear_down(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total) {
    return amp_max * time_remain / time_total;
}

std::uint32_t buzzer_amp_function_max(std::uint32_t amp_max, std::uint32_t time_remain, std::uint32_t time_total) {
    return amp_max;
}

void Buzzer::playEmptyNote(std::uint32_t duration_ms) {
    std::uint32_t
        clock_freq  = HAL_RCC_GetHCLKFreq(),
        prescaler   = tim_freq_handler.Instance->PSC,
        auto_reload = __HAL_TIM_CALC_PERIOD(clock_freq, prescaler, 1000 * 2);
    __HAL_TIM_SetAutoreload(&tim_freq_handler, auto_reload);
    HAL_TIM_OC_Stop(&tim_amp_handler, tim_amp_channel);
}

void Buzzer::playNote(const BuzzerNote &note) {
    if (note.freq == 0) {
        playEmptyNote(note.duration_ms);
        return;
    }

    std::uint32_t
        clock_freq  = HAL_RCC_GetHCLKFreq(),
        prescaler   = tim_freq_handler.Instance->PSC,
        auto_reload = __HAL_TIM_CALC_PERIOD(clock_freq, prescaler, note.freq * 2);

    __HAL_TIM_SetAutoreload(&tim_freq_handler, auto_reload);

    current_state.tick_remain = note.freq * note.duration_ms / 1000;
    current_state.tick_total  = current_state.tick_remain;
    current_state.max_compare = __HAL_TIM_GetAutoreload(&tim_amp_handler) * clamp(0.f, note.amp, 1.f) * volume;

    auto amp_compare = calc_amp_compare(note, current_state.max_compare, current_state.tick_total, current_state.tick_remain);
    __HAL_TIM_SetCompare(&tim_amp_handler, tim_amp_channel, amp_compare);
    HAL_TIM_OC_Start(&tim_amp_handler, tim_amp_channel);
}

std::uint32_t Buzzer::calc_amp_compare(const BuzzerNote &note, std::uint32_t max_compare, std::uint32_t tick_total, std::uint32_t tick_remain) {
    return note.amp_function(max_compare, tick_remain, tick_total);
}

const BuzzerNote &Buzzer::get_current_note() {
    return current_state.sequence[current_state.current_position];
}

void Buzzer::freq_cycle_half() {
    current_state.cycle_end = false;
    __HAL_TIM_SetCompare(&tim_amp_handler, tim_amp_channel, 0);
}

void Buzzer::freq_cycle_end() {
    current_state.cycle_end = true;
    current_state.tick_remain--;
    auto compare = calc_amp_compare(get_current_note(), current_state.max_compare, current_state.tick_total, current_state.tick_remain);
    __HAL_TIM_SetCompare(&tim_amp_handler, tim_amp_channel, compare);
    if (current_state.tick_remain != 0)
        return;
    current_state.current_position++;
    if (current_state.current_position == current_state.sequence_length) {
        stop();
        return;
    }
    playNote(get_current_note());
}

void Buzzer::play(const BuzzerNoteSequence &seq) {
    stop();
    if (seq.length == 0)
        return;
    current_state.current_position = 0;
    current_state.sequence         = seq.sequence;
    current_state.sequence_length  = seq.length;
    playNote(seq.sequence[0]);
    HAL_TIM_Base_Start(&tim_amp_handler);
    HAL_TIM_Base_Start_IT(&tim_freq_handler);
}

void Buzzer::stop() {
    __HAL_TIM_SetCompare(&tim_amp_handler, tim_amp_channel, 0);
    HAL_TIM_Base_Stop(&tim_amp_handler);
    HAL_TIM_OC_Stop(&tim_amp_handler, tim_amp_channel);
    HAL_TIM_Base_Stop_IT(&tim_freq_handler);
}

void Buzzer::on_tim_freq_period_elapsed(void) {
    if (current_state.cycle_end == false)
        freq_cycle_end();
    else
        freq_cycle_half();
}

void Buzzer::set_volume(float volume) {
    this->volume = clamp(0.f, volume, 1.f);
}
