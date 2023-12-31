#include "UTF8Praser.hpp"

std::uint32_t UTF8Praser::nextUTF8(const char *str, std::size_t *charLen) {
    std::uint32_t result = 0;
    std::size_t chLen = 0;
    do {
        result <<= 8;
        result |= *str;
        chLen ++;
    } while ((*(str++) & 0x80) != 0);
    if(charLen)
        *charLen = chLen;
    return result;
}