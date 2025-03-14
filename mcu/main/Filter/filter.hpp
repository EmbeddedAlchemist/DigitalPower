#pragma once

template <typename DataType = float>
class Filter {
  public:
    virtual DataType filter(DataType value) = 0;
};

template <typename DataType = float>
inline DataType operator>>(DataType value, Filter<DataType> &filter) {
    return filter.filter(value);
}

