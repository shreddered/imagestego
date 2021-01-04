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
#include "imagestego/algorithms/dwt.hpp"
// c++ headers
#include <iostream>


namespace {

void dwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.copyTo(dst);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<short>(i, (j << 1)),
                 b = src.at<short>(i, (j << 1) + 1);
            dst.at<short>(i, j) = (a + b) / 2;
            dst.at<short>(i, j + x) = a - b;
        }
}

void idwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.copyTo(dst);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<short>(i, j),
                 b = src.at<short>(i, j + x);
            dst.at<short>(i, (j << 1)) = a + ((b + 1 < 0 && (b + 1) % 2) ? b : b + 1) / 2;
            dst.at<short>(i, (j << 1) + 1) = a - ((b < 0 && b % 2) ? b - 1 : b) / 2;
        }
}

} // namespace

namespace imagestego {

void dwt(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_16S);
    cv::Mat tmp;
    dwt1D(dst, tmp);
    dwt1D(tmp.t(), dst);
    dst = dst.t();
}

void idwt(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat tmp;
    idwt1D(src.t(), tmp);
    idwt1D(tmp.t(), dst);
    dst.convertTo(dst, CV_8U);
}

DwtEmbedder<void>::DwtEmbedder() noexcept {}

DwtEmbedder<void>::DwtEmbedder(const std::string& imageName, const std::string& output)
    : image(cv::imread(imageName)), outputFile(output) {}


void DwtEmbedder<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void DwtEmbedder<void>::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void DwtEmbedder<void>::setSecretKey(const std::string& _key) {
    key = hash(_key);
    // seeding PRNG
    gen.seed(key);
    hasKey = true;
}

void DwtEmbedder<void>::setMessage(const std::string& _msg) {
    msg = BitArray<>(_msg);
}

void DwtEmbedder<void>::createStegoContainer() {
    if (!hasKey) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        auto s = keygen::generate();
        std::cout << "key = " << s << std::endl;
        setSecretKey(s);
#else
        throw Exception(Exception::Codes::NoKeyFound);
#endif
    }
    uint32_t sz = 4 * ceil(sqrt(msg.size()));
    std::size_t currentMsgIndex = 0;
    auto arr = BitArray<>::fromInt(sz);
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    for (int i = 0; i != dwtGreen.rows && currentMsgIndex != 32; ++i)
        for (int j = 0; j != dwtGreen.cols && currentMsgIndex != 32; ++j) {
            if (arr[currentMsgIndex++])
                dwtGreen.at<short>(i, j) |= 1;
            else
                dwtGreen.at<short>(i, j) &= ~1;
        }
    cv::Mat tmp;
    idwt(dwtGreen, tmp);
    tmp.copyTo(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)));
    // after that, perform embedding
    int x0 = 0, y0 = 0;
    do {
        y0 = gen() % image.rows;
        x0 = gen() % image.cols;
    } while (y0 > image.rows - sz || x0 > image.cols - sz);
    cv::Mat cropped = planes[0](cv::Rect(x0, y0, sz, sz));
    BitArray<> sizeBitStream;
    sizeBitStream.pushBack(msg.size(), 32);
    msg = sizeBitStream + msg;
    // DWT of blue channel
    dwt(cropped, tmp);
    currentMsgIndex = 0;
    int i = tmp.rows >> 1,
        j = tmp.cols >> 1;
    currentMsgIndex = 0;
    for (i = tmp.rows >> 1; i != tmp.rows && currentMsgIndex != msg.size(); ++i) {
        for (j = tmp.cols >> 1; j != tmp.cols && currentMsgIndex != msg.size(); ++j) {
            if (msg[currentMsgIndex++])
                tmp.at<short>(i, j) |= 1;
            else
                tmp.at<short>(i, j) &= ~1;
        }
    }
    idwt(tmp, cropped);
    cropped.copyTo(planes[0](cv::Rect(x0, y0, sz, sz)));
    cv::merge(planes, image);
    cv::imwrite(outputFile, image);
}

DwtExtracter<void>::DwtExtracter() noexcept {}

DwtExtracter<void>::DwtExtracter(const std::string& imageName) : image(cv::imread(imageName)) {}

void DwtExtracter<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void DwtExtracter<void>::setSecretKey(const std::string& _key) {
    key = hash(_key);
    // seeding PRNG
    gen.seed(key);
    hasKey = true;
}

std::string DwtExtracter<void>::extractMessage() {
    if (!key)
        throw Exception(Exception::Codes::NoKeyFound);
    BitArray<unsigned char> arr;
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    BitArray<uint32_t> arr1;
    for (int i = 0; i != dwtGreen.rows; ++i)
        for (int j = 0; j != dwtGreen.cols; ++j)
            arr1.pushBack((dwtGreen.at<short>(i, j) & 1) != 0);
    uint32_t sz = arr1.getBlock(0);
    int x0 = 0, y0 = 0;
    do {
        y0 = gen() % image.rows;
        x0 = gen() % image.cols;
    } while (y0 > image.rows - sz || x0 > image.cols - sz);
    cv::Mat cropped = planes[0](cv::Rect(x0, y0, sz, sz));
    std::size_t currentMsgIndex = 0,
                msgSize = 0;
    bool sizeKnown = false;
    // DWT of blue channel
    cv::Mat tmp;
    dwt(cropped, tmp);
    for (int i = tmp.rows >> 1; i != tmp.rows; ++i)
        for (int j = tmp.cols >> 1; j != tmp.cols; ++j) {
            ++currentMsgIndex;
            bool bit = (tmp.at<short>(i, j) & 1) != 0;
            if (currentMsgIndex > 32) {
                sizeKnown = true;
                arr.pushBack(bit);
            }
            else
                msgSize |= bit << (32 - currentMsgIndex);
            if (sizeKnown && currentMsgIndex - 32 == msgSize)
                return arr.toString();
        }
}

Algorithm DwtEmbedder<void>::getAlgorithm() const noexcept {
    return Algorithm::Dwt;
}

Algorithm DwtExtracter<void>::getAlgorithm() const noexcept {
    return Algorithm::Dwt;
}

} // namespace imagestego
