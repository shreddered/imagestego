#include "imagestego/space_check.hpp"

#if __cplusplus > 201403L
#   define static_if(cond) if constexpr (cond)
#else
#   define static_if(cond) if (cond)
#endif 


namespace imagestego {

void spaceCheck(const std::size_t& msgSize, const cv::Mat& img, Algorithm alg) {
    static_if(alg == Algorithm::Dwt) {
        auto size = img.cols * img.rows / 4;
        if (msgSize > size)
            throw Exception(Exception::Codes::BigMessageSize);
    }
    else static_if(alg == Algorithm::Lsb) {
        auto size = img.cols * img.rows;
        if (msgSize > size)
            throw Exception(Exception::Codes::BigMessageSize);
    }
}

void spaceCheck(const std::size_t& msgSize, const JpegImage& img, Algorithm alg) {
    static_if(alg == Algorithm::JpegLsb) {        
        auto size = img.cols * img.rows;
        if (msgSize > size)
            throw Exception(Exception::Codes::BigMessageSize);
    }
    else static_if(alg == Algorithm::F3) {
    }
}

} // namespace imagestego
