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

// imagestego headers
#include "imagestego/algorithm/wavelet.hpp"
#include "imagestego/core.hpp"
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <algorithm>
#include <random>
// opencv headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace imagestego {

namespace impl {

cv::Rect selectRect(const cv::Mat& src, std::mt19937& gen,
                    std::size_t minRectArea) {
    int x0, y0, x1, y1;
    do {
        auto x = std::minmax({gen() % src.cols, gen() % src.cols}),
             y = std::minmax({gen() % src.rows, gen() % src.rows});
        x0 = x.first;
        x1 = x.second;
        y0 = y.first;
        y1 = y.second;
    } while ((x1 - x0) * (y1 - y0) < minRectArea && !(y0 == 0 && x0 >= 0 && x0 <= 11));
    return cv::Rect(cv::Point(x0, y0), cv::Point(x1, y1));
}

class WaveletEmbedder {
public:
    explicit WaveletEmbedder(Wavelet* wavelet, Encoder* encoder)
        : _encoder(encoder), _wavelet(wavelet) {}
    virtual ~WaveletEmbedder() noexcept {
        delete _wavelet;
        if (_encoder)
            delete _encoder;
    }
    void setImage(const std::string& src) { _image = cv::imread(src); }
    void setMessage(const std::string& msg) {
        if (_encoder) {
            _encoder->setMessage(msg);
        } else {
            _arr = imagestego::BitArray::fromByteString(msg);
        }
    }
    void setSecretKey(const std::string& key) {
        _prng.seed(imagestego::hash(key));
    }
    void createStegoContainer(const std::string& dst) {
        std::size_t idx = 0;
        imagestego::BitArray size = imagestego::BitArray::fromInt(_arr.size());
        for (int row = 0; row < _image.rows && idx < 32; ++row) {
            for (int col = 0; col < _image.cols && idx < 32; ++col) {
                auto p = _image.at<cv::Vec3b>(row, col);
                for (int color = 0; color < 3 && idx < 32; ++color) {
                    if (size[idx]) {
                        p.val[color] |= 1;
                    } else {
                        p.val[color] &= ~1u;
                    }
                    ++idx;
                }
                _image.at<cv::Vec3b>(row, col) = p;
            }
        }
        auto rect = selectRect(_image, _prng, _arr.size() * 4);
        cv::Mat transformed = _wavelet->transform(_image(rect));
        idx = 0;
        for (int row = transformed.rows / 2; row < transformed.rows && idx < _arr.size(); ++row) {
            for (int col = transformed.cols / 2; col < transformed.cols && idx < _arr.size(); ++col) {
                auto p = transformed.at<cv::Vec3s>(row, col);
                for (int color = 0; color != 3 && idx < _arr.size(); ++color) {
                    if (_arr[idx]) {
                        p.val[color] |= 1;
                    } else {
                        p.val[color] &= ~1u;
                    }
                    ++idx;
                }
                transformed.at<cv::Vec3s>(row, col) = p;
            }
        }
        _wavelet->inverse(transformed).copyTo(_image(rect));
        cv::imwrite(dst, _image);
    }

private:
    cv::Mat _image;
    imagestego::BitArray _arr;
    std::mt19937 _prng;
    Encoder* _encoder;
    Wavelet* _wavelet;
}; // class WaveletEmbedder

class WaveletExtracter {
public:
    explicit WaveletExtracter(Wavelet* wavelet, Decoder* decoder)
        : _wavelet(wavelet), _decoder(decoder) {}
    virtual ~WaveletExtracter() noexcept {

    }
    void setImage(const std::string& src) {
        _image = cv::imread(src);
    }
    void setSecretKey(const std::string& key) {
        _prng.seed(imagestego::hash(key));
    }
    std::string extractMessage() {
        imagestego::BitArray msg, _size;
        std::size_t idx = 0;
        for (int row = 0; row < _image.rows && idx < 32; ++row) {
            for (int col = 0; col < _image.cols && idx < 32; ++col) {
                auto p = _image.at<cv::Vec3b>(row, col);
                for (int color = 0; color != 3 && idx < 32; ++color) {
                    _size.pushBack((p.val[color] & 1) != 0);
                    ++idx;
                }
            }
        }
        std::size_t size = _size.toInt();
        idx = 0;
        auto rect = selectRect(_image, _prng, size * 4);
        cv::Mat transformed = _wavelet->transform(_image(rect));
        for (int row = transformed.rows / 2; row < transformed.rows && idx < size; ++row) {
            for (int col = transformed.cols / 2; col < transformed.cols && idx < size; ++col) {
                auto p = transformed.at<cv::Vec3s>(row, col);
                for (int color = 0; color != 3 && idx < size; ++color) {
                    msg.pushBack((p.val[color] & 1) != 0);
                    ++idx;
                }
            }
        }
        if (_decoder) {
            _decoder->setMessage(msg);
            return _decoder->getDecodedMessage();
        } else {
            return msg.toByteString();
        }
    }
private:
    std::mt19937 _prng;
    cv::Mat _image;
    Wavelet* _wavelet;
    Decoder* _decoder;
}; // class WaveletExtracter

} // namespace impl

WaveletEmbedder::WaveletEmbedder(Wavelet* wavelet, Encoder* encoder)
    : _pImpl(new impl::WaveletEmbedder(wavelet, encoder)) {}

WaveletEmbedder::~WaveletEmbedder() noexcept {
    if (_pImpl)
        delete _pImpl;
}

void WaveletEmbedder::setImage(const std::string& src) { _pImpl->setImage(src); }

void WaveletEmbedder::setMessage(const std::string& msg) { _pImpl->setMessage(msg); }

void WaveletEmbedder::setSecretKey(const std::string& key) { _pImpl->setSecretKey(key); }

void WaveletEmbedder::createStegoContainer(const std::string& dst) {
    _pImpl->createStegoContainer(dst);
}

WaveletExtracter::WaveletExtracter(Wavelet* wavelet, Decoder* decoder)
    : _pImpl(new impl::WaveletExtracter(wavelet, decoder)) {}

WaveletExtracter::~WaveletExtracter() noexcept {
    if (_pImpl)
        delete _pImpl;
}

void WaveletExtracter::setImage(const std::string& src) { _pImpl->setImage(src); }

void WaveletExtracter::setSecretKey(const std::string& key) {
    _pImpl->setSecretKey(key);
}

std::string WaveletExtracter::extractMessage() {
    return _pImpl->extractMessage();
}

} // namespace imagestego
