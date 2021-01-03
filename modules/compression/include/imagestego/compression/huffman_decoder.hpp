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

#ifndef __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__
#define __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <string>
#include <unordered_map>
#include <vector>

namespace imagestego {

class HuffmanDecoderImpl;

class IMAGESTEGO_EXPORTS HuffmanDecoder : public AbstractDecoder {
public:
    explicit HuffmanDecoder() noexcept;
    explicit HuffmanDecoder(const BitArray& arr) noexcept;
    virtual ~HuffmanDecoder() noexcept;
    void setMessage(const BitArray& arr) override;
    std::string getDecodedMessage() override;

private:
    HuffmanDecoderImpl* decoder;
}; // class HuffmanDecoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__ */
