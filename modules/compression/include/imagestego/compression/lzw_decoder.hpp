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

#ifndef __LZW_DECODER_HPP_INCLUDED__
#define __LZW_DECODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"
// c++ headers
#include <string>


namespace imagestego {

class LzwDecoderImpl : private Dictionary {
public:
    static constexpr uint8_t maxBits = 12;
    explicit LzwDecoderImpl() noexcept;
    explicit LzwDecoderImpl(const BitArray& str) noexcept;
    void setMessage(const BitArray& arr) noexcept {
        msg = arr;
        decodedMsg.clear();
    }
    std::string getDecodedMessage();
private:
    std::string decodedMsg;
    mutable BitArray msg;
    void decode();
}; // class LzwDecoderImpl

class IMAGESTEGO_EXPORTS LzwDecoder : public AbstractDecoder {
public:
    explicit LzwDecoder();
    explicit LzwDecoder(const BitArray& arr);
    virtual ~LzwDecoder() noexcept;
    void setMessage(const BitArray& arr) override;
    std::string getDecodedMessage() override;
private:
    LzwDecoderImpl* _decoder;
}; // class LzwDecoder

} // namespace imagestego 

#endif /* __LZW_DECODER_HPP_INCLUDED__ */
