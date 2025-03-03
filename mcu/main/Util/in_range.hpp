#pragma once


template <typename NumberType>
bool in_range(NumberType range_min, NumberType val, NumberType range_max){
    return range_min <= val && val <= range_max;
}