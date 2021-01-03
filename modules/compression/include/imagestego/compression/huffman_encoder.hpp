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

#ifndef __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__
#define __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__

// imagestego
#include "imagestego/core.hpp"
//#include "imagestego/utils/bitarray.hpp"
// c++
#include <algorithm>
#include <cstddef>
#include <map>
#include <string>

namespace imagestego {

class HuffmanEncoderImpl;

class IMAGESTEGO_EXPORTS HuffmanEncoder : public AbstractEncoder {
public:
    explicit HuffmanEncoder() noexcept;
    explicit HuffmanEncoder(const std::string& str) noexcept;
    explicit HuffmanEncoder(std::string&& str) noexcept;
    HuffmanEncoder(const HuffmanEncoder&) = delete;
    HuffmanEncoder& operator=(const HuffmanEncoder&) = delete;
    void setMessage(const std::string& str) override;
    BitArray getEncodedMessage() override;
    virtual ~HuffmanEncoder() noexcept;

private:
    HuffmanEncoderImpl* encoder;
}; // class HuffmanEncoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__ */
