#pragma once
#include "filter.hpp"
#include <limits>

class PID : public Filter<float> {
  private:
    float kp;
    float ki;
    float kd;

    float ui;
    float target;

    float out_min;
    float out_max;

  public:
    inline PID(float kp, float ki, float kd, float out_min = std::numeric_limits<float>::min(), float out_max = std::numeric_limits<float>::max())
        : kp(kp),
          ki(ki),
          kd(kd),
          out_min(out_min),
          out_max(out_max) {}

    virtual float filter(float value) override final;
    inline void set_target(float target) { this->target = target; }
    inline void set_out_min(float out_min) { this->out_min = out_min; }
    inline void set_out_max(float out_max) { this->out_min = out_max; }
    inline void set_kp(float kp) { this->kp = kp; }
    inline void set_ki(float ki) { this->ki = ki; }
    inline void set_kd(float kd) { this->kd = kd; }
};