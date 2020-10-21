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

#ifndef __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/config.hpp"
// c++
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS AbstractStegoEmbedder {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setMessage(const std::string& msg) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual void createStegoContainer(const std::string& dst) = 0;
    virtual ~AbstractStegoEmbedder() = default;
}; // class AbstractStegoEmbedder

class IMAGESTEGO_EXPORTS AbstractStegoExtracter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual std::string extractMessage() = 0;
    virtual ~AbstractStegoExtracter() = default;
}; // class AbstractStegoExtracter

class IMAGESTEGO_EXPORTS AbstractEncoder {
public:
    virtual void setMessage(const std::string& msg) = 0;
    virtual BitArray getEncodedMessage() = 0;
    virtual ~AbstractEncoder() noexcept = default;
}; // class AbstractEncoder

class IMAGESTEGO_EXPORTS AbstractDecoder {
public:
    virtual void setMessage(const BitArray& arr) = 0;
    virtual std::string getDecodedMessage() = 0;
    virtual ~AbstractDecoder() noexcept = default;
}; // class AbstractDecoder

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_INTERFACES_HPP_INCLUDED__ */
