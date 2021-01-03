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
#include "imagestego/compression/lzw_dictionary.hpp"


namespace imagestego {

class LzwEncoderImpl : private Dictionary {
public:
    explicit LzwEncoderImpl() noexcept : Dictionary() {}
    explicit LzwEncoderImpl(const std::string& str) noexcept : Dictionary(), _msg(str) {}
    void setMessage(const std::string& str) noexcept {
        _msg = str;
        _encodedMsg.clear();
    }
    BitArray getEncodedMessage() {
        if (_encodedMsg.empty() && _msg.size() != 1) {
            encode();
        }
        return _encodedMsg;
    }
private:
    static constexpr std::size_t maxDictionarySize = (1 << maxBits) - 1;
    std::string _msg;
    BitArray _encodedMsg;
    void encode() {
        StringElement s;
        uint8_t currentBitsPerBlock = 8;
        std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
        _encodedMsg.put(maxBits, 4);
        for (std::size_t i = 0; i != _msg.size(); ++i) {
            s.value = _msg[i];
            int index = Dictionary::search(s);
            if (index != -1) {
                s.prefixIndex = index;
            }
            else {
                _encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
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
        _encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
    }
}; // class LzwEncoderImpl

LzwEncoder::LzwEncoder() : _encoder(new LzwEncoderImpl) {}

LzwEncoder::LzwEncoder(const std::string& str) : _encoder(new LzwEncoderImpl(str)) {}

LzwEncoder::~LzwEncoder() noexcept {
    if (_encoder)
        delete _encoder;
}

void LzwEncoder::setMessage(const std::string& str) {
    _encoder->setMessage(str);
}

BitArray LzwEncoder::getEncodedMessage() {
    return _encoder->getEncodedMessage();
}

} // namespace imagestego
