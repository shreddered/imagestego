#ifndef __IMAGESTEGO_DWT_HPP_INCLUDED__
#define __IMAGESTEGO_DWT_HPP_INCLUDED__

// imagestego
#include <core.hpp>
#include <utils/bitarray.hpp>
// c++
#include <algorithm>
#include <string>
#include <vector>
// opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


namespace imagestego {

void dwt(const cv::Mat& src, cv::Mat& dst);

class DwtStegoInserter : public AbstractStegoInserter {
public:
    explicit DwtStegoInserter() noexcept;
    explicit DwtStegoInserter(const std::string& image, const std::string& output);
    void setImage(const std::string& imageName) override;
    void setOutputName(const std::string& filename) override;
    void setMessage(const std::string& msg) override;
    void createStegoContainer() const override;
private:
    cv::Mat image;
    BitArray<> msg;
}; // class DwtStegoInserter

class DwtStegoExtracter : public AbstractStegoExtracter {
public:
    explicit DwtStegoExtracter() noexcept;
    explicit DwtStegoExtracter(const std::string& image);
    void setImage(const std::string& imageName) override;
    std::string extractMessage() override;
private:
    cv::Mat image;
}; // class DwtStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_DWT_HPP_INCLUDED__ */
