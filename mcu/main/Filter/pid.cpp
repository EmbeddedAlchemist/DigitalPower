#include "pid.hpp"
#include "clamp.hpp"

float PID::filter(float real) {
    float error = target - real;
    float up    = kp * error;
    ui          = clamp(out_min, ui + (ki * up), out_max);
    float ud = kd * ui;
    return clamp(out_min, up + ui + ud, out_max);
}