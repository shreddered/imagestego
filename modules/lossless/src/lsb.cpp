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

// imagestego headers
#include "imagestego/algorithm/lsb.hpp"
#include "route.hpp"
// opencv headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace imagestego {

class LsbEmbedderImpl final {
public:
    explicit LsbEmbedderImpl(AbstractEncoder* encoder = nullptr) noexcept
        : _encoder(encoder) {}
    virtual ~LsbEmbedderImpl() noexcept {
        if (_encoder)
            delete _encoder;
    }
    void setImage(const std::string& src) { _image = cv::imread(src); }
    void setMessage(const std::string& msg) {
        if (_encoder) {
            _encoder->setMessage(msg);
            _msg = _encoder->getEncodedMessage();
        } else {
            _msg = BitArray::fromByteString(msg);
        }
    }
    void setSecretKey(const std::string& key) {
        _key = BitArray::fromByteString(key);
        _gen.seed(hash(key));
    }
    void createStegoContainer(const std::string& dst) {
        if (_key.empty())
            throw Exception(Exception::Codes::NoKeyFound);
        BitArray sz = BitArray::fromInt(_msg.size());
        std::size_t idx = 0, i = 0;
        Route r(std::make_pair(_image.cols, _image.rows), _gen);
        r.create(32);
        for (auto it = r.begin(); it != r.end(); ++it) {
            auto& pix = _image.at<cv::Vec3b>(it->second, it->first);
            bool b = (pix.val[0] & 1u) != 0;
            if (b != _key[idx]) {
                if (sz[i])
                    pix.val[1] |= 1u;
                else
                    pix.val[1] &= ~1u;
            } else {
                if (sz[i])
                    pix.val[2] |= 1u;
                else
                    pix.val[2] &= ~1u;
            }
            idx = (idx + 1) % _key.size();
            ++i;
        }

        Route r1(r.begin(), r.end(), _gen);
        r1.setMapSize(std::make_pair(_image.cols, _image.rows));
        r1.create(32 + _msg.size());
        i = 0;
        for (auto it = r1.begin(); it != r1.end(); ++it) {
            if (r.search(*it))
                continue;
            auto& pixel = _image.at<cv::Vec3b>(it->second, it->first);
            bool b = (pixel.val[0] & 1) != 0;
            if (b != _key[idx]) {
                if (_msg[i])
                    pixel.val[1] |= 1;
                else
                    pixel.val[1] &= ~1u;
            } else {
                if (_msg[i])
                    pixel.val[2] |= 1u;
                else
                    pixel.val[2] &= ~1u;
            }
            idx = (idx + 1) % _key.size();
            ++i;
        }
        cv::imwrite(dst, _image);
    }

private:
    AbstractEncoder* _encoder = nullptr;
    /** PRNG */
    std::mt19937 _gen;
    /** image */
    cv::Mat _image;
    BitArray _key, _msg;
}; // class LsbEmbedderImpl

class LsbExtracterImpl final {
public:
    explicit LsbExtracterImpl(AbstractDecoder* decoder = nullptr) noexcept
        : _decoder(decoder) {}
    virtual ~LsbExtracterImpl() noexcept {
        if (_decoder)
            delete _decoder;
    }
    void setImage(const std::string& src) { _image = cv::imread(src); }
    void setSecretKey(const std::string& key) {
        _key = BitArray::fromByteString(key);
        _gen.seed(hash(key));
    }
    std::string extractMessage() {
        if (_key.empty())
            throw Exception(Exception::Codes::NoKeyFound);
        BitArray sz;
        std::size_t idx = 0;
        Route r(std::make_pair(_image.cols, _image.rows), _gen);
        r.create(32);
        for (auto it = r.begin(); it != r.end(); ++it) {
            auto pix = _image.at<cv::Vec3b>(it->second, it->first);
            bool b = (pix.val[0] & 1u) != 0;
            if (_key[idx] != b)
                sz.pushBack((pix.val[1] & 1u) != 0);
            else
                sz.pushBack((pix.val[2] & 1u) != 0);
            idx = (idx + 1) % _key.size();
        }
        auto size = sz.toInt();
        BitArray msg;
        Route r1(r.begin(), r.end(), _gen);
        r1.setMapSize(std::make_pair(_image.cols, _image.rows));
        r1.create(32 + size);
        for (auto it = r1.begin(); it != r1.end(); ++it) {
            if (r.search(*it))
                continue;
            auto pixel = _image.at<cv::Vec3b>(it->second, it->first);
            bool b = (pixel.val[0] & 1u) != 0;
            if (_key[idx] != b)
                msg.pushBack((pixel.val[1] & 1u) != 0);
            else
                msg.pushBack((pixel.val[2] & 1u) != 0);
            idx = (idx + 1) % _key.size();
        }
        if (_decoder) {
            _decoder->setMessage(msg);
            return _decoder->getDecodedMessage();
        } else {
            return msg.toByteString();
        }
    }

private:
    AbstractDecoder* _decoder;
    std::mt19937 _gen;
    cv::Mat _image;
    BitArray _key;
}; // class LsbExtracterImpl

// LsbEmbedder
LsbEmbedder::LsbEmbedder(AbstractEncoder* encoder)
    : _embedder(new LsbEmbedderImpl(encoder)) {}

LsbEmbedder::~LsbEmbedder() noexcept {
    if (_embedder)
        delete _embedder;
}

void LsbEmbedder::setImage(const std::string& src) { _embedder->setImage(src); }

void LsbEmbedder::setMessage(const std::string& msg) { _embedder->setMessage(msg); }

void LsbEmbedder::setSecretKey(const std::string& key) { _embedder->setSecretKey(key); }

void LsbEmbedder::createStegoContainer(const std::string& dst) {
    _embedder->createStegoContainer(dst);
}

// LsbExtracter
LsbExtracter::LsbExtracter(AbstractDecoder* decoder)
    : _extracter(new LsbExtracterImpl(decoder)) {}

LsbExtracter::~LsbExtracter() noexcept {
    if (_extracter)
        delete _extracter;
}

void LsbExtracter::setImage(const std::string& src) { _extracter->setImage(src); }

void LsbExtracter::setSecretKey(const std::string& key) { _extracter->setSecretKey(key); }

std::string LsbExtracter::extractMessage() { return _extracter->extractMessage(); }

} // namespace imagestego
