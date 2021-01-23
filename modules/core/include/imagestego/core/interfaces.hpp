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

#ifndef __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/config.hpp"
// c++
#include <string>

namespace imagestego {

/**
 * @brief Interface for custom stego embedders.
 */
class IMAGESTEGO_EXPORTS StegoEmbedder {
public:
    /**
     * @brief Source image setter.
     */
    virtual void setImage(const std::string& imageName) = 0;
    /**
     * @brief Secret message setter.
     */
    virtual void setMessage(const std::string& msg) = 0;
    /**
     * @brief Setter for secret key.
     */
    virtual void setSecretKey(const std::string& key) = 0;
    /**
     * @brief Method for creating stego container.
     */
    virtual void createStegoContainer(const std::string& dst) = 0;
    virtual ~StegoEmbedder() = default;
}; // class StegoEmbedder

/**
 * @brief Interface for custom stego extracters.
 */
class IMAGESTEGO_EXPORTS StegoExtracter {
public:
    /**
     * @brief Source image setter.
     */
    virtual void setImage(const std::string& imageName) = 0;
    /**
     * @brief Setter for secret key.
     */
    virtual void setSecretKey(const std::string& key) = 0;
    /**
     * @brief Function for extracting secret message.
     */
    virtual std::string extractMessage() = 0;
    virtual ~StegoExtracter() = default;
}; // class StegoExtracter

/**
 * @brief Interface for custom encoders.
 */
class IMAGESTEGO_EXPORTS Encoder {
public:
    /**
     * @brief Setter for message to be encoded.
     */
    virtual void setMessage(const std::string& msg) = 0;
    /**
     * @brief Getter for encoded message.
     */
    virtual BitArray getEncodedMessage() = 0;
    virtual ~Encoder() noexcept = default;
}; // class Encoder

/**
 * @brief Interface for custom decoders.
 */
class IMAGESTEGO_EXPORTS Decoder {
public:
    /**
     * @brief Encoded message setter.
     */
    virtual void setMessage(const BitArray& arr) = 0;
    /**
     * @brief Getter for decoded message.
     */
    virtual std::string getDecodedMessage() = 0;
    virtual ~Decoder() noexcept = default;
}; // class Decoder

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__ */
