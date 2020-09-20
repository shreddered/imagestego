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

#include "imagestego/compression/lzw_decoder.hpp"

#include <iostream>


int read(const imagestego::BitArray& arr, std::size_t& i, uint8_t bits) {
    int block = 0;
    std::size_t it;
    for (it = i; it != i + bits; ++it)
        block |= arr[it] << (bits - 1 - (it - i)); 
    i = it;
    return block;
}

namespace imagestego {

LzwDecoderImpl::LzwDecoderImpl() noexcept : Dictionary() {}

LzwDecoderImpl::LzwDecoderImpl(const BitArray& arr) noexcept : Dictionary(), msg(arr) {}

std::string LzwDecoderImpl::getDecodedMessage() {
    if (decodedMsg.empty()) {
        decode();
    }
    return decodedMsg;
}

void LzwDecoderImpl::decode() {
    // reading first 4 bits    
    std::size_t i = 0;
    uint8_t maxBits = read(msg, i, 4);
    while(i < msg.size()) {
        Dictionary::clear();
        uint8_t currentBitsPerBlock = 8;
        std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
        std::size_t code = read(msg, i, currentBitsPerBlock);
        decodedMsg += code;
        std::size_t oldCode = code;
        while (1) {
            if (i >= msg.size())
                break;
            if (size() == currentMaxDictionarySize) {
                if (currentBitsPerBlock == maxBits)
                    break;
                ++currentBitsPerBlock;
                currentMaxDictionarySize = (1 << currentBitsPerBlock);
            }
            code = read(msg, i, currentBitsPerBlock);
            std::string tmp;
            if (code < Dictionary::size()) {
                tmp = Dictionary::at(code);
                decodedMsg += tmp;
            }
            else {
                tmp = Dictionary::at(oldCode);
                decodedMsg += tmp + tmp[0];
            }
            Dictionary::add(tmp[0], oldCode);
            oldCode = code;
        }
    }
}

LzwDecoder::LzwDecoder() : _decoder(new LzwDecoderImpl) {}

LzwDecoder::LzwDecoder(const BitArray& arr) : _decoder(new LzwDecoderImpl(arr)) {}

LzwDecoder::~LzwDecoder() noexcept {
    delete _decoder;
}

void LzwDecoder::setMessage(const BitArray& arr) {
    _decoder->setMessage(arr);
}

std::string LzwDecoder::getDecodedMessage() {
    return _decoder->getDecodedMessage();
}

} // namespace imagestego
