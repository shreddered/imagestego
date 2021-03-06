/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// imagestego headers
#include "imagestego/compression/lzw_encoder.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"

namespace imagestego {

namespace impl {

class LzwEncoder : private Dictionary {
public:
    explicit LzwEncoder() noexcept : Dictionary() {}
    explicit LzwEncoder(const std::string& str) noexcept : Dictionary(), _msg(str) {}
    void setMessage(const std::string& str) noexcept {
        _msg = str;
        _encodedMsg.clear();
    }
    imagestego::BitArray getEncodedMessage() {
        if (_encodedMsg.empty() && _msg.size() != 1) {
            encode();
        }
        return _encodedMsg;
    }

private:
    static constexpr std::size_t maxDictionarySize = (1 << maxBits) - 1;
    std::string _msg;
    imagestego::BitArray _encodedMsg;
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
            } else {
                _encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
                s.prefixIndex = s.value;
                if (Dictionary::size() > currentMaxDictionarySize) {
                    if (currentBitsPerBlock == maxBits) {
                        currentBitsPerBlock = 8;
                        Dictionary::clear();
                    } else
                        ++currentBitsPerBlock;
                    currentMaxDictionarySize = (1 << currentBitsPerBlock);
                }
            }
        }
        _encodedMsg.put(s.prefixIndex, currentBitsPerBlock);
    }
}; // class LzwEncoder

} // namespace impl

LzwEncoder::LzwEncoder() : _encoder(new impl::LzwEncoder) {}

LzwEncoder::LzwEncoder(const std::string& str) : _encoder(new impl::LzwEncoder(str)) {}

LzwEncoder::~LzwEncoder() noexcept {
    if (_encoder)
        delete _encoder;
}

void LzwEncoder::setMessage(const std::string& str) { _encoder->setMessage(str); }

BitArray LzwEncoder::getEncodedMessage() { return _encoder->getEncodedMessage(); }

} // namespace imagestego
