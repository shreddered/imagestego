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

#ifndef __IMAGESTEGO_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_LSB_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/interfaces.hpp"
// c++ headers
#include <random>
#include <string>
#include <type_traits>

namespace imagestego {

class LsbEmbedderImpl;

class LsbExtracterImpl;

/**
 * Class for performing LSB-based embedding.
 */
class IMAGESTEGO_EXPORTS LsbEmbedder : public AbstractStegoEmbedder {
public:
    /**
     * Constructs embedder with given encoder.
     *
     * @param encoder Encoder which will process data.
     */
    explicit LsbEmbedder(AbstractEncoder* encoder = nullptr);

    /**
     * LsbEmbedder destructror.
     */
    virtual ~LsbEmbedder() noexcept;

    /**
     * Setter for source image.
     *
     * @param src Path to image.
     */
    void setImage(const std::string& src) override;

    /**
     * Setter for message.
     *
     * @param msg Message string.
     */
    void setMessage(const std::string& msg) override;

    /**
     * Setter for secret key.
     *
     * @param src Secret key string.
     */
    void setSecretKey(const std::string& key) override;

    /**
     * Performs embedding.
     *
     * @param dst Path to new image.
     */
    void createStegoContainer(const std::string& dst) override;

private:
    LsbEmbedderImpl* _embedder;
}; // class LsbEmbedder

class IMAGESTEGO_EXPORTS LsbExtracter : public AbstractStegoExtracter {
public:
    /**
     * Constructs extracter with given decoder.
     *
     * @param decoder Decoder of extracted data.
     */
    explicit LsbExtracter(AbstractDecoder* decoder = nullptr);

    /**
     * LsbExtracter destructror.
     */
    virtual ~LsbExtracter() noexcept;

    /**
     * Setter for stego container.
     *
     * @param src Path to image with stego message.
     */
    void setImage(const std::string& dst) override;

    /**
     * Sets secret key.
     *
     * @param key Secret key.
     */
    void setSecretKey(const std::string& key) override;

    /**
     * Extracts message from image.
     *
     * @return Extracted message.
     */
    std::string extractMessage() override;

private:
    LsbExtracterImpl* _extracter;
}; // class LsbExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_LSB_HPP_INCLUDED__ */
