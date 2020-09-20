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

// imagestego headers
#include "imagestego/compression/lzw_encoder.hpp"
// c++ headers
#include <iostream>


namespace imagestego {

LzwEncoder::LzwEncoder() noexcept : Dictionary() {}

LzwEncoder::LzwEncoder(const std::string& str) noexcept : Dictionary(), msg(str) {}

LzwEncoder::LzwEncoder(std::string&& str) noexcept : Dictionary(), msg(str) {}

void LzwEncoder::setMessage(const std::string& str) noexcept {
    msg = str;
    encodedMsg.clear();
}

void LzwEncoder::setMessage(std::string&& str) noexcept {
    msg = str;
    encodedMsg.clear();
}

BitArray LzwEncoder::getEncodedMessage() {
    if (encodedMsg.empty() && msg.size() != 1) {
        encode();
    }
    return encodedMsg;
}

void LzwEncoder::encode() {
    StringElement s;
    uint8_t currentBitsPerBlock = 8;
    std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
    encodedMsg.put(maxBits, 4); 
    for (std::size_t i = 0; i != msg.size(); ++i) {
        s.value = msg[i];
        int index = Dictionary::search(s);
        if (index != -1) {
            s.prefixIndex = index;
        }
        else {
            encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
            s.prefixIndex = s.value;
            if (Dictionary::size() > currentMaxDictionarySize) { 
                if (currentBitsPerBlock == maxBits) {
                    currentBitsPerBlock = 8;
                    Dictionary::clear();
                }
                else
                    ++currentBitsPerBlock;
                currentMaxDictionarySize = (1 << currentBitsPerBlock);
            }
        }
    }
    encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
}

} // namespace imagestego
