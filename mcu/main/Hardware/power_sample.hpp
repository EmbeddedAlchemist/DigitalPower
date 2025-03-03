#pragma once

#include "hal_header.hpp"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <limits> // for

class PowerSample {
  private:
    static constexpr float
        vref                             = 3.3,
        adc_sample_to_voltage_multiplier = 3.3f / 65535,
        vin_dc_multiplier                = adc_sample_to_voltage_multiplier / (1100.f / (1100.f + 9900.f)),
        vin_pd_multiplier                = adc_sample_to_voltage_multiplier / (1100.f / (1100.f + 9900.f)),
        vin_multiplier                   = adc_sample_to_voltage_multiplier / (1100.f / (1100.f + 9900.f)),
        vout_multiplier                  = adc_sample_to_voltage_multiplier / (1100.f / (1100.f + 9900.f)),
        iin_multiplier                   = adc_sample_to_voltage_multiplier * 0.005f * 220000.f / 1000.f ,
        iout_multiplier                  = adc_sample_to_voltage_multiplier * 0.005f * 220000.f / 1000.f;

    ADC_HandleTypeDef &adc1_handle;
    ADC_HandleTypeDef &adc2_handle;
    OPAMP_HandleTypeDef &opamp_iout_handle;
    OPAMP_HandleTypeDef &opamp_iin_handle;

    union {
        std::uint16_t adc_buffer[6] = {};
        struct {
            std::uint16_t vin_sample;    // ADC1_IN1
            std::uint16_t vin_pd_sample; // ADC1_IN3
            std::uint16_t vin_dc_sample; // ADC1_IN4
            std::uint16_t vout_sample;   // ADC2 IN13
            std::uint16_t iout_sample;   // OPAMP2 COMP4
            std::uint16_t iin_sample;    // OPAMP3 COMP1
        };
    };

    static constexpr std::uint32_t
        gain_max = 64,
        gain_min = 1;

    static constexpr float
        gain_raise_threshold = 0.3,
        gain_down_threshold  = 0.7;

    std::uint32_t iout_sample_gain = 2;
    std::uint32_t iin_sample_gain  = 2;

    void opamp_set_gain(OPAMP_HandleTypeDef &opamp, std::uint32_t gain);

  public:
    inline constexpr PowerSample(
        ADC_HandleTypeDef &adc1_handle,
        ADC_HandleTypeDef &adc2_handle,
        OPAMP_HandleTypeDef &opamp2_handle,
        OPAMP_HandleTypeDef &opamp3_handle)
        : adc1_handle(adc1_handle),
          adc2_handle(adc2_handle),
          opamp_iout_handle(opamp2_handle),
          opamp_iin_handle(opamp3_handle) {}

    void init();
    void handle_auto_gain(void);

    float get_dc_voltage();
    float get_pd_voltage();
    float get_in_voltage();
    float get_out_voltage();
    float get_in_current();
    float get_out_current();
};