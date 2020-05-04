#ifndef __IMAGESTEGO_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_LSB_HPP_INCLUDED__

// imagestego
#include <core.hpp>
#include <utils/avl_tree.hpp>
#include <utils/bitarray.hpp>
// c++
#include <random>
#include <string>
#include <stdexcept>
#include <vector>
// opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


namespace imagestego {

enum LsbOptions {
    silly = 0,
    randomBits = 1, // will be using PRNG
    usePlusMinusOne = (1 << 1) | 1
};

class LsbInsertionError : public std::runtime_error {
public:
    explicit LsbInsertionError(const std::string& msg) noexcept;
};

class LsbExtractionError : public std::runtime_error {
public:
    explicit LsbExtractionError(const std::string& msg) noexcept;
};

namespace {

inline cv::Point fromPair(typename Route::iterator it) {
    return cv::Point(it->first, it->second);
}

}

namespace __ {

void change(uint8_t& val) noexcept;

}

template<class EncoderType>
class LsbInserter : public AbstractStegoInserter {
public:
    explicit LsbInserter(const int& _opts = LsbOptions::silly) noexcept : opts(_opts) {}
    explicit LsbInserter(const std::string& imageName, const std::string& output, const int& _opts = LsbOptions::silly) 
        : image(cv::imread(imageName)), outputFile(output), opts(_opts) {}
    void setImage(const std::string& imageName) override {
        image = cv::imread(imageName);
    }
    void setOutputName(const std::string& filename) override {
        outputFile = filename;
    }
    void setMessage(const std::string& _msg) noexcept override {
        encoder.setMessage(_msg);
        msg = encoder.getEncodedMessage();
    }
    void setSecretKey(const std::string& _key) noexcept {
        key = BitArray<unsigned int>(_key);
    }
    void createStegoContainer() const override {
        switch(opts) {
            case 0:
                __sillyLsbInsertion();
                break;
            case 1:
                __randomLsbInsertion(0);
                break;
            case 3:
                __randomLsbInsertion(1);
                break;
            default:
                break;
        }
    }
private:
    void __sillyLsbInsertion() const {
        std::size_t currentBitIndex = 0;
        msg.put('\0');
        for (int row = 0; row != image.rows && currentBitIndex != msg.size(); ++row) {
            for (int col = 0; col != image.cols && currentBitIndex != msg.size(); ++col) {
                for (uint8_t color = 0; color != 3; ++color) {
                    auto pixel = image.at<cv::Vec3b>(row, col);
                    if (msg[currentBitIndex++])
                        pixel.val[color] |= 1u;
                    else
                        pixel.val[color] &= ~1u;
                    image.at<cv::Vec3b>(row, col) = pixel;
                }
            }
        }
        cv::imwrite(outputFile, image);
    }
    void __randomLsbInsertion(bool flag) const {
        if (key.empty())
            throw LsbInsertionError("No key found");
        seed();
        Route route(std::make_pair(image.cols, image.rows));
        route.create(32);
        auto it = route.begin();
        BitArray<std::size_t> tmp = BitArray<std::size_t>::fromInt(msg.size());
        std::size_t currentKeyIndex = 0;
        // writing size
        for (int i = 0; i != 32; ++i, ++it) {
            auto p = fromPair(it);
            auto& pixel = image.at<cv::Vec3b>(p.y, p.x);
            // pixel's LSB = pixel.val[0] & 1
            bool bit = tmp[i];
            if ((pixel.val[0] & 1u) != key[currentKeyIndex % key.size()]) {
                if (flag)
                    __::change(pixel[1]);
                if (bit)
                    pixel.val[1] |= 1u;
                else
                    pixel.val[1] &= ~1u;
            }
            else {
                if (flag)
                    __::change(pixel[2]);
                if (bit)
                    pixel.val[2] |= 1u;
                else
                    pixel.val[2] &= ~1u;
            }
            ++currentKeyIndex;
        }
        Route r1(route.begin(), route.end());
        r1.setMapSize(std::make_pair(image.cols, image.rows));
        r1.create(32 + msg.size());
        std::size_t i = 0;
        for (auto it = r1.begin(); it != r1.end(); ++it) {
            if (route.search(*it))
                continue;
            auto p = fromPair(it);
            auto& pixel = image.at<cv::Vec3b>(p.y, p.x);
            // pixel's LSB = pixel.val[0] & 1
            bool bit = msg[i];
            if ((pixel.val[0] & 1u) != key[currentKeyIndex % key.size()]) {
                if (flag)
                    __::change(pixel[1]);
                if (bit)
                    pixel.val[1] |= 1u;
                else
                    pixel.val[1] &= ~1u;
            }
            else {
                if (flag)
                    __::change(pixel[2]);
                if (bit)
                    pixel.val[2] |= 1u;
                else
                    pixel.val[2] &= ~1u;
            }
            ++currentKeyIndex;
            ++i;
        }
        cv::imwrite(outputFile, image);
    }
    mutable EncoderType encoder;
    inline void seed() const noexcept {
        srand(key.getBlock(0));
    }
    int opts;
    mutable cv::Mat image;
    std::string outputFile;
    mutable BitArray<> msg;
    BitArray<unsigned int> key;
};

template<class DecoderType>
class LsbExtracter : public AbstractStegoExtracter {
public:
    explicit LsbExtracter(const int& _opts = LsbOptions::silly) noexcept : opts(_opts) {}
    explicit LsbExtracter(const std::string& imageName, const int& _opts = LsbOptions::silly) 
        : image(cv::imread(imageName)), opts(_opts) {}
    void setImage(const std::string& imageName) override {
        image = cv::imread(imageName);
    }
    void setSecretKey(const std::string& _key) noexcept {
        key = BitArray<unsigned int>(_key);
    }
    std::string extractMessage() override {
        switch(opts) {
            case 0:
                return __sillyLsbExtraction();
            case 1:
#if __cpluspus >= 201103L
                [[fallthrough]];
#endif
            case 3:
                return __randomLsbExtraction();
            default:
                return "";
        }
    }
private:
    std::string __sillyLsbExtraction() const {
        BitArray<unsigned char> arr;
        for (int row = 0; row != image.rows; ++row) {
            for (int col = 0; col != image.cols; ++col) {
                for (uint8_t color = 0; color != 3; ++color) {
                    const auto& pixel = image.at<cv::Vec3b>(row, col);
                    bool b = (pixel.val[color] & 1u) != 0;
                    arr.pushBack(b);
                    if (arr.size() % 8 == 0 && arr.lastBlock() == 0) {
                        return arr.toString();
                    }
                }
            }
        }
    }
    std::string __randomLsbExtraction() const {
        if (key.empty())
            throw LsbExtractionError("No key found");
        seed();
        BitArray<uint8_t> arr;
        BitArray<std::size_t> tmp;
        Route r(std::make_pair(image.cols, image.rows));
        r.create(32);
        std::size_t currentKeyIndex = 0;
        for (auto it = r.begin(); it != r.end(); ++it) {
            auto point = fromPair(it);
            auto pixel = image.at<cv::Vec3b>(point.y, point.x);
            bool b = (pixel.val[0] & 1u) != 0;
            if (key[currentKeyIndex] != b) // green case
                tmp.pushBack((pixel.val[1] & 1u) != 0);
            else 
                tmp.pushBack((pixel.val[2] & 1u) != 0);
            currentKeyIndex = (currentKeyIndex + 1) % key.size();
        }
        std::size_t size = tmp.getBlock(0);
        Route r1(r.begin(), r.end());
        r1.setMapSize(std::make_pair(image.cols, image.rows));
        r1.create(32 + size);
        for (auto it = r1.begin(); it != r1.end(); ++it) {
            if (r.search(*it))
                continue;
            auto point = fromPair(it);
            auto pixel = image.at<cv::Vec3b>(point.y, point.x);
            bool b = (pixel.val[0] & 1u) != 0;
            if (key[currentKeyIndex] != b) // green case
                arr.pushBack((pixel.val[1] & 1u) != 0);
            else
                arr.pushBack((pixel.val[2] & 1u) != 0);
            currentKeyIndex = (currentKeyIndex + 1) % key.size();
        }
        decoder.setMessage(arr);
        return decoder.getDecodedMessage();
    }
    inline void seed() const noexcept {
        srand(key.getBlock(0));
    }
    int opts;
    cv::Mat image;
    BitArray<unsigned int> key;
    mutable DecoderType decoder;
}; // class

template<>
class LsbInserter<void> : public AbstractStegoInserter {
public:
    explicit LsbInserter(const int& _opts = LsbOptions::silly) noexcept;
    explicit LsbInserter(const std::string& imageName, const std::string& output, const int& _opts = LsbOptions::silly); 
    void setImage(const std::string& imageName) override; 
    void setOutputName(const std::string& filename) override;
    void setMessage(const std::string& _msg) noexcept override;
    void setSecretKey(const std::string& _key) noexcept;
    void createStegoContainer() const override; 
private:
    void __sillyLsbInsertion() const;
    void __randomLsbInsertion(bool flag) const;
    inline void seed() const noexcept {
        srand(key.getBlock(0));
    }
    int opts;
    mutable cv::Mat image;
    std::string outputFile;
    mutable BitArray<> msg;
    BitArray<unsigned int> key;
}; // inserter

template<>
class LsbExtracter<void> : public AbstractStegoExtracter {
public:
    explicit LsbExtracter(const int& _opts = LsbOptions::silly) noexcept;
    explicit LsbExtracter(const std::string& imageName, const int& _opts = LsbOptions::silly); 
    void setImage(const std::string& imageName) override;
    void setSecretKey(const std::string& _key) noexcept; 
    std::string extractMessage() override; 
private:
    std::string __sillyLsbExtraction() const;
    std::string __randomLsbExtraction() const; 
    inline void seed() const noexcept {
        srand(key.getBlock(0));
    }
    int opts;
    cv::Mat image;
    BitArray<unsigned int> key;
}; // class LsbExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_LSB_HPP_INCLUDED__ */
