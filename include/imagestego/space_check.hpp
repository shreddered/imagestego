#ifndef __IMAGESTEGO_SPACE_CHECK_HPP_INCLUDED__
#define __IMAGESTEGO_SPACE_CHECK_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/jpeg_image.hpp"
// opencv headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


namespace imagestego {

template<class ImageType>
void spaceCheck(const std::size_t& msgSize, const ImageType& image, Algorithm alg);

void spaceCheck(const std::size_t& msgSize, const cv::Mat& img, Algorithm alg);

void spaceCheck(const std::size_t& msgSize, const JpegImage& image, Algorithm alg);

}; // namespace

#endif /* __IMAGESTEGO_SPACE_CHECK_HPP_INCLUDED__ */
