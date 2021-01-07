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

#include "imagestego/compression/lzw_decoder.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"

std::size_t read(const imagestego::BitArray& arr, std::size_t& i, uint8_t bits) {
    std::size_t block = 0;
    std::size_t it;
    for (it = i; it != i + bits; ++it)
        block |= arr[it] << (bits - 1 - (it - i));
    i = it;
    return block;
}

namespace imagestego {

class LzwDecoderImpl : private Dictionary {
public:
    static constexpr uint8_t maxBits = 12;
    explicit LzwDecoderImpl() noexcept : Dictionary() {}
    explicit LzwDecoderImpl(const BitArray& arr) noexcept : Dictionary(), _msg(arr) {}
    void setMessage(const BitArray& arr) noexcept {
        _msg = arr;
        _decodedMsg.clear();
    }
    std::string getDecodedMessage() {
        if (_decodedMsg.empty()) {
            decode();
        }
        return _decodedMsg;
    }

private:
    std::string _decodedMsg;
    BitArray _msg;
    void decode() {
        // reading first 4 bits
        std::size_t i = 0;
        uint8_t maxBits = read(_msg, i, 4);
        while (i < _msg.size()) {
            Dictionary::clear();
            uint8_t currentBitsPerBlock = 8;
            std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
            std::size_t code = read(_msg, i, currentBitsPerBlock);
            _decodedMsg += static_cast<char>(code); // always 8-bit
            std::size_t oldCode = code;
            while (1) {
                if (i >= _msg.size())
                    break;
                if (size() == currentMaxDictionarySize) {
                    if (currentBitsPerBlock == maxBits)
                        break;
                    ++currentBitsPerBlock;
                    currentMaxDictionarySize = (1 << currentBitsPerBlock);
                }
                code = read(_msg, i, currentBitsPerBlock);
                std::string tmp;
                if (code < Dictionary::size()) {
                    tmp = Dictionary::at(code);
                    _decodedMsg += tmp;
                } else {
                    tmp = Dictionary::at(oldCode);
                    _decodedMsg += tmp + tmp[0];
                }
                Dictionary::add(tmp[0], oldCode);
                oldCode = code;
            }
        }
    }
}; // class LzwDecoderImpl

LzwDecoder::LzwDecoder() : _decoder(new LzwDecoderImpl) {}

LzwDecoder::LzwDecoder(const BitArray& arr) : _decoder(new LzwDecoderImpl(arr)) {}

LzwDecoder::~LzwDecoder() noexcept { delete _decoder; }

void LzwDecoder::setMessage(const BitArray& arr) { _decoder->setMessage(arr); }

std::string LzwDecoder::getDecodedMessage() { return _decoder->getDecodedMessage(); }

} // namespace imagestego
