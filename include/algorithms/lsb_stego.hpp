#ifndef __LSB_STEGO_HPP_INCLUDED__
#define __LSB_STEGO_HPP_INCLUDED__

#include <algorithms/base_interfaces.hpp>
#include <utils/bitarray.hpp>

#include <random>
#include <string>
#include <vector>

#include <opencv2/core.hpp>


enum LsbOptions {
    silly = 0,
    randomBits = 1, // will be using PRNG
    randomChanging = 1 << 1 // +- 1
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
    void setStegoContainer(const std::string& filename) override;
    std::string extractMessage() override;
private:
    cv::Point generatePoint() const noexcept;
    void __sillyLsbInsertion() const;
    LsbOptions opts;
    mutable cv::Mat image;
    Mode mode;
    std::string outputFile;
    mutable BitArray<> msg;
};


#endif /* __LSB_STEGO_HPP_INCLUDED__ */
