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

namespace impl {

class HuffmanEncoder;

} // namespace impl

/**
 * @brief Class implementing Huffman coding.
 */
class IMAGESTEGO_EXPORTS HuffmanEncoder : public Encoder {
public:
    /**
     * @brief Constructs empty HuffmanEncoder instance.
     */
    explicit HuffmanEncoder() noexcept;
    /**
     * @brief Constructs HuffmanEncoder instance with given message.
     *
     * @param str Message to be encoded.
     */
    explicit HuffmanEncoder(const std::string& str) noexcept;
    explicit HuffmanEncoder(std::string&& str) noexcept;
    HuffmanEncoder(const HuffmanEncoder&) = delete;
    HuffmanEncoder& operator=(const HuffmanEncoder&) = delete;
    /**
     * @brief Setter for message to be encoded.
     *
     * @param str Message to be encoded.
     */
    void setMessage(const std::string& str) override;
    /**
     * @brief Getter for encoded message.
     *
     * @return Encoded message.
     */
    BitArray getEncodedMessage() override;
    /**
     * @brief Destructs HuffmanEncoder.
     */
    virtual ~HuffmanEncoder() noexcept;

private:
    impl::HuffmanEncoder* encoder;
}; // class HuffmanEncoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__ */
