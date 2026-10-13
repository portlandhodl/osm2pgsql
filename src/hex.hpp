#ifndef OSM2PGSQL_HEX_HPP
#define OSM2PGSQL_HEX_HPP

/**
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This file is part of osm2pgsql (https://osm2pgsql.org/).
 *
 * Copyright (C) 2006-2026 by the osm2pgsql developer community.
 * For a full list of authors see the git log.
 */

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace util {

/**
 * Convert content of input string to hex and append to output.
 *
 * \param in The input data.
 * \param out Pointer to output string.
 */
void encode_hex(std::string const &in, std::string *out);

/**
 * Convert content of input string to hex and return it.
 *
 * \param in The input data.
 * \returns Hex encoded string.
 */
[[nodiscard]] std::string encode_hex(std::string const &in);

namespace detail {

/// Lookup table mapping each character to its hex value (0 for non-hex).
inline constexpr std::array<char, 256> HEX_DECODE_TABLE = {
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1,  2,  3,  4,  5,  6,  7, 8, 9, 0, 0, 0, 0, 0, 0,

    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

} // namespace detail

/**
 * Decode one hex character (0-9A-F or 0-9a-f) and return its value.
 * Returns 0 for characters that are not hex characters.
 */
[[nodiscard]] inline unsigned char decode_hex_char(char c) noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return detail::HEX_DECODE_TABLE[static_cast<std::size_t>(
        static_cast<unsigned char>(c))];
}

/**
 * Decode a string of hex characters. Throws an exception if the input is not
 * a valid hex encoding.
 */
[[nodiscard]] std::string decode_hex(std::string_view hex);

} // namespace util

#endif // OSM2PGSQL_HEX_HPP
