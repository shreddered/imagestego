#ifndef __LSB_STEGO_HPP_INCLUDED__
#define __LSB_STEGO_HPP_INCLUDED__

#include <bitarray.hpp>

#include <string>

#include <highgui.h>


enum LsbOptions {
    silly = 0,
    randomBits = 1, // will be using PRNG
    randomChanging = 1 << 1 // +- 1
};

class Lsb : public AbstractStegoExtracter, public AbstractStegoInserter {
public:
    explicit Lsb() noexcept;
    explicit Lsb(const LsbOptions& _opts) noexcept;
    void setImage(const std::string& imageName) override;
    void setImage(const cv::Mat& _image) noexcept override;
    void setOutputName(const std::string& filename) override;
    void createStegoContainer() override; 
    void setMessage(const std::string& msg) noexcept override;
    void setStegoContainer(const std::string& filename) override;
    std::string extractMessage() override;
private:
    cv::Point generatePoint();
    LsbOptions opts;
    cv::Mat image;
    Mode mode;
    std::string outputFile;
    BitArray<> msg;
};


#endif /* __LSB_STEGO_HPP_INCLUDED__ */
