#include "imagestego/space_check.hpp"

#if __cplusplus > 201403L
#   define static_if(cond) if constexpr (cond)
#else
#   define static_if(cond) if (cond)
#endif 


std::size_t countLsbValid(const imagestego::JpegImage& image) {
    std::size_t res = 0;
    for (int i = 0; i != image.rows; ++i) {
        for (int j = 0; j != image.cols; ++j) {
            auto p = image.at(i, j);
            for (char k = 0; k != 3; ++k)
                if (p[k] != 0 && p[k] != -1 && p[k] != 1)
                    ++res;
        }
    }
    return res;
}

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

} // namespace imagestego
