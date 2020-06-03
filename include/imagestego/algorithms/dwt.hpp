#ifndef __IMAGESTEGO_DWT_HPP_INCLUDED__
#define __IMAGESTEGO_DWT_HPP_INCLUDED__

// imagestego
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
#   include "imagestego/keygen.hpp"
#   include <iostream>
#endif
#ifdef IMAGESTEGO_ENABLE_FORMAT_CHECKNG
#   include "imagestego/utils/format_checker.hpp"
#endif
// c++
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
// opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
// murmurhash
#include "MurmurHash3.h"


namespace imagestego {

IMAGESTEGO_EXPORTS void dwt(const cv::Mat& src, cv::Mat& dst);

IMAGESTEGO_EXPORTS void idwt(const cv::Mat& src, cv::Mat& dst);

template<class EncoderType>
class DwtEmbedder : public AbstractStegoEmbedder {
public:
    explicit DwtEmbedder() noexcept {}
    explicit DwtEmbedder(const std::string& imageName, const std::string& output) 
        : image(cv::imread(imageName)), outputFile(output) {}
    void setImage(const std::string& imageName) override {
        image = cv::imread(imageName); 
    }
    void setOutputName(const std::string& filename) override {
        outputFile = filename;
    }
    void setMessage(const std::string& _msg) override {
        encoder.setMessage(_msg);
        msg = encoder.getEncodedMessage();
    }
    void setSecretKey(const std::string& _key) override {
        key = hash(_key);
    }
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::Dwt;
    }
    void createStegoContainer() const override {
        if (!key) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
            auto s = keygen::generate();
            std::cout << "key = " << s << endl;
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
        // seeding PRNG
        gen.seed(key);
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
private:
    EncoderType encoder;
    cv::Mat image;
    std::string outputFile;
    mutable std::mt19937 gen;
    mutable BitArray<> msg;
    uint32_t key;
}; // class DwtEmbedder

template<class DecoderType>
class DwtExtracter : public AbstractStegoExtracter {
public:
    explicit DwtExtracter() noexcept {}
    explicit DwtExtracter(const std::string& imageName) : image(cv::imread(imageName)) {} 
    void setImage(const std::string& imageName) override {
        image = cv::imread(imageName);
    }
    void setSecretKey(const std::string& _key) override { 
        key = hash(_key);
    }
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::Dwt;
    }
    std::string extractMessage() override {
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
        // seeding PRNG
        gen.seed(key);
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
                if (sizeKnown && currentMsgIndex - 32 == msgSize) {
                    decoder.setMessage(arr);
                    return decoder.getDecodedMessage();
                }
            }
    }
private:
    std::mt19937 gen;
    cv::Mat image;
    DecoderType decoder;
    uint32_t key;
}; // class DwtExtracter

template<>
class IMAGESTEGO_EXPORTS DwtEmbedder<void> : public AbstractStegoEmbedder {
public:
    explicit DwtEmbedder() noexcept;
    explicit DwtEmbedder(const std::string& imageName, const std::string& output);
    void setImage(const std::string& imageName) override;
    void setOutputName(const std::string& filename) override;
    void setMessage(const std::string& msg) override;
    void setSecretKey(const std::string& key) override;
    void createStegoContainer() const override;
    Algorithm getAlgorithm() const noexcept override;
private:
    cv::Mat image;
    std::string outputFile;
    mutable std::mt19937 gen;
    mutable BitArray<> msg;
    uint32_t key;
}; // class DwtStegoEmbedder

template<>
class IMAGESTEGO_EXPORTS DwtExtracter<void> : public AbstractStegoExtracter {
public:
    explicit DwtExtracter() noexcept;
    explicit DwtExtracter(const std::string& image);
    void setImage(const std::string& imageName) override;
    void setSecretKey(const std::string& key) override;
    std::string extractMessage() override;
    Algorithm getAlgorithm() const noexcept override;
private:
    std::mt19937 gen;
    cv::Mat image;
    uint32_t key;
}; // class DwtStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_DWT_HPP_INCLUDED__ */
