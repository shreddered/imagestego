#ifndef __IMAGESTEGO_WAVELET_INTERFACES_HPP_INCLUDED__
#define __IMAGESTEGO_WAVELET_INTERFACES_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// opencv headers
#include <opencv2/core/mat.hpp>


namespace imagestego {

class IMAGESTEGO_EXPORTS AbstractWavelet {
public:
    virtual cv::Mat transform(const cv::Mat& src) = 0;
    virtual cv::Mat inverse(const cv::Mat& src) = 0;
    virtual ~AbstractWavelet() noexcept = default;
}; // class AbstractWaveletTransform

} // namespace imagestego

#endif /* __IMAGESTEGO_WAVELET_INTERFACES_HPP_INCLUDED__ */
