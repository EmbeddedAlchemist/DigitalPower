#pragma once

#include "util/macros.hpp"

#include "Offset.hpp"
#include "mono_bitmap.hpp"

_NAMESPACE_EXGRAPHICS_BEGIN

class FontCharacter : public MonoBitmap {
  public:
    /**
     * @brief UTF8 code of the character
     *
     */
    std::uint32_t charater;

    /**
     * @brief Y direction of the character
     *
     */
    std::int16_t offset_y;
    constexpr FontCharacter(std::uint32_t charater, const std::uint32_t *bitmap_data, Size size, std::int16_t offset_y)
        : MonoBitmap(bitmap_data, size), offset_y(offset_y), charater(charater) {};
};

class Font {
  protected:
    /**
     * @brief pointer to font character array;
     *  ** this array must be sorted by character code because of find method is using binary search **
     */
    const FontCharacter *characters;

    /**
     * @brief number of characters, aka elements count of characters array
     *
     */
    const std::size_t characters_count;

  public:
    /**
     * @brief height of font ascent
     *
     */
    const std::uint16_t ascent_height;

    /**
     * @brief height of font descent
     * hua
     */
    const std::uint16_t descent_height;

    const std::size_t grayscale_bits;

    constexpr Font(
        const FontCharacter *characters,
        std::size_t count_of_characters,
        std::uint16_t ascent_height,
        std::uint16_t descent_height,
        std::size_t grayscale_bits)
        : characters(characters),
          characters_count(count_of_characters),
          ascent_height(ascent_height),
          descent_height(descent_height),
          grayscale_bits(grayscale_bits) {};

    /**
     * @brief find FontCharacter with utf8 code in characters array.
     *
     * @param character utf8 code
     * @return const FontCharacter* null if not found;
     */
    const FontCharacter *find(std::uint32_t character) const;
};

_NAMESPACE_EXGRAPHICS_END