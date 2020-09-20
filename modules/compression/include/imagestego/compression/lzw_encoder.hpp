/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++ headers
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS LzwEncoder : private Dictionary {
public:
    explicit LzwEncoder() noexcept;
    explicit LzwEncoder(const std::string& str) noexcept;
    explicit LzwEncoder(std::string&& str) noexcept;
    void setMessage(const std::string& str) noexcept;
    void setMessage(std::string&& str) noexcept;
    BitArray<> getEncodedMessage();
private:
    static constexpr std::size_t maxDictionarySize = (1 << maxBits) - 1; 
    std::string msg;
    mutable BitArray<> encodedMsg;
    void encode();
}; // class LzwEncoder

} // namespace imagestego 

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
