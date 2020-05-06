#ifndef __IMAGESTEGO_DWT_HPP_INCLUDED__
#define __IMAGESTEGO_DWT_HPP_INCLUDED__

// imagestego
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
#   include "imagestego/keygen.hpp"
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

class IMAGESTEGO_EXPORTS DwtEmbedder : public AbstractStegoEmbedder {
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

class IMAGESTEGO_EXPORTS DwtExtracter : public AbstractStegoExtracter {
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
