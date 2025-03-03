#pragma once

template <typename number_type>
inline constexpr number_type clamp(number_type min, number_type val, number_type max) {
    return val < min   ? min
           : val > max ? max
                       : val;
}