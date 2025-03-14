#include "filter.hpp"

class LowPassFilter : public Filter<float> {
    float a;
    float val;

  public:
    inline LowPassFilter(float a, float inital_val)
        : a(a), val(inital_val) {}

    virtual float filter(float value) override final;
};