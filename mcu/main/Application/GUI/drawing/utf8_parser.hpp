#pragma once

#include <cstdint>
#include <cstddef>

class UTF8Parser{
    private:
      static std::size_t get_char_length(const char *str);

    public:
      static std::uint32_t next_char(const char *str, std::size_t *char_len);
};