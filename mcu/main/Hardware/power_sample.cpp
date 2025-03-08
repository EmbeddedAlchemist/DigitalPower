#include "power_sample.hpp"


void PowerSample::opamp_set_gain(OPAMP_HandleTypeDef &opamp, std::uint32_t gain) {
    switch (gain) {
    case 1:
        LL_OPAMP_SetMode(opamp.Instance, LL_OPAMP_MODE_FOLLOWER);
        break;
    default:
        LL_OPAMP_SetMode(opamp.Instance, LL_OPAMP_MODE_PGA);
        break;
    }
    switch (gain) {
    case 2:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_2_OR_MINUS_1);
        break;
    case 4:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_4_OR_MINUS_3);
        break;
    case 8:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_8_OR_MINUS_7);
        break;
    case 16:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_16_OR_MINUS_15);
        break;
    case 32:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_32_OR_MINUS_31);
        break;
    case 64:
        LL_OPAMP_SetPGAGain(opamp.Instance, LL_OPAMP_PGA_GAIN_64_OR_MINUS_63);
        break;
    }
}

void PowerSample::init() {
    HAL_OPAMP_Start(&opamp_iout_handle);
    HAL_OPAMP_Start(&opamp_iin_handle);
    HAL_ADC_Start_DMA(&adc1_handle, reinterpret_cast<std::uint32_t *>(&adc_buffer[0]), 3);
    HAL_ADC_Start_DMA(&adc2_handle, reinterpret_cast<std::uint32_t *>(&adc_buffer[3]), 3);
}

void PowerSample::handle_auto_gain(void) {
    auto handle_single_channel = [this](uint16_t &sample_value, uint32_t &sample_gain, OPAMP_HandleTypeDef &opamp_handle) {
        const auto
            adc_sample_gain_down_threshold  = std::numeric_limits<std::uint16_t>::max() * gain_down_threshold,
            adc_sample_gain_raise_threshold = std::numeric_limits<std::uint16_t>::min() * gain_raise_threshold;

        if (sample_value > adc_sample_gain_down_threshold && sample_gain > gain_min) {
            opamp_set_gain(opamp_handle, sample_gain >>= 1);
            sample_value >>= 1;
        } else if (vout_sample < adc_sample_gain_raise_threshold && sample_gain < gain_min) {
            opamp_set_gain(opamp_handle, sample_gain <<= 1);
            sample_value <<= 1;
        }
    };

    handle_single_channel(iin_sample, iin_sample_gain, opamp_iin_handle);
    handle_single_channel(iout_sample, iout_sample_gain, opamp_iout_handle);
}

float PowerSample::get_dc_voltage() {
    return static_cast<float>(vin_dc_sample) * vin_dc_multiplier;
}

float PowerSample::get_pd_voltage() {
    return static_cast<float>(vin_pd_sample) * vin_pd_multiplier;
}

float PowerSample::get_in_voltage() {
    return static_cast<float>(vin_sample) * vin_multiplier;
}

float PowerSample::get_out_voltage() {
    return static_cast<float>(vout_sample) * vin_multiplier;
}

float PowerSample::get_in_current() {
    return static_cast<float>(iin_sample) / iin_sample_gain * iin_multiplier;
}

float PowerSample::get_out_current() {
    return static_cast<float>(iout_sample) / iout_sample_gain * iout_multiplier;
}
