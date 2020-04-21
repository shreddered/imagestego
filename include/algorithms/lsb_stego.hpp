#ifndef __LSB_STEGO_HPP_INCLUDED__
#define __LSB_STEGO_HPP_INCLUDED__

#include <core.hpp>
#include <utils/bitarray.hpp>

#include <random>
#include <string>
#include <stdexcept>
#include <vector>

#include <opencv2/core.hpp>


enum LsbOptions {
    silly = 0,
    randomBits = 1, // will be using PRNG
    usePlusMinusOne = (1 << 1) | 1
};

class Lsb : public AbstractStegoExtracter, public AbstractStegoInserter {
public:
    explicit Lsb() noexcept;
    explicit Lsb(const LsbOptions& _opts) noexcept;
    explicit Lsb(const std::string& imageName, const std::string& output, const LsbOptions& _opts = LsbOptions::silly);
    explicit Lsb(const std::string& output);
    void setImage(const std::string& imageName) override;
    void setOutputName(const std::string& filename) override;
    void createStegoContainer() const override; 
    void setMessage(const std::string& msg) noexcept override;
    template<typename Int>
    void setMessage(const BitArray<Int>& arr) noexcept {
        this->msg = arr;
    }
    void setStegoContainer(const std::string& filename) override;
    void setSecretKey(const std::string& key) noexcept;
    std::string extractMessage() override;
private:
    static void change(uint8_t& val) noexcept;
    cv::Point generatePoint() const noexcept;
    void __sillyLsbInsertion() const;
    void __randomLsbInsertion(bool) const;
    std::string __sillyLsbExtraction() const;
    std::string __randomLsbExtraction() const;
    void seed() const noexcept;
    LsbOptions opts;
    mutable cv::Mat image;
    std::string outputFile;
    mutable BitArray<> msg;
    mutable BitArray<unsigned int> key;
};

class LsbInsertionError : public std::runtime_error {
public:
    explicit LsbInsertionError(const std::string& msg) noexcept;
};

#endif /* __LSB_STEGO_HPP_INCLUDED__ */
