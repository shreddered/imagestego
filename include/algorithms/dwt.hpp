#ifndef __IMAGESTEGO_DWT_HPP_INCLUDED__
#define __IMAGESTEGO_DWT_HPP_INCLUDED__

// imagestego
#include "core.hpp"
#include "utils/bitarray.hpp"
// c++
#include <algorithm>
#include <string>
#include <vector>
// opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


namespace imagestego {

IMAGESTEGO_EXPORTS void dwt(const cv::Mat& src, cv::Mat& dst);

IMAGESTEGO_EXPORTS void idwt(const cv::Mat& src, cv::Mat& dst);

class IMAGESTEGO_EXPORTS DwtEmbedder : public AbstractStegoEmbedder {
public:
    explicit DwtEmbedder() noexcept;
    explicit DwtEmbedder(const std::string& imageName, const std::string& output);
    void setImage(const std::string& imageName) override;
    void setOutputName(const std::string& filename) override;
    void setMessage(const std::string& msg) override;
    void createStegoContainer() const override;
private:
    cv::Mat image;
    std::string outputFile;
    mutable BitArray<> msg;
}; // class DwtStegoEmbedder

class IMAGESTEGO_EXPORTS DwtExtracter : public AbstractStegoExtracter {
public:
    explicit DwtExtracter() noexcept;
    explicit DwtExtracter(const std::string& image);
    void setImage(const std::string& imageName) override;
    std::string extractMessage() override;
private:
    cv::Mat image;
}; // class DwtStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_DWT_HPP_INCLUDED__ */
