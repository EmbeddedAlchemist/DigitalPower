#include "low_pass_filter.hpp"


float LowPassFilter::filter(float value){
    return val * (1.f - a) + value * a;
}