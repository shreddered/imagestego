// Example of Haar wavelet transform on image
// Usage:
//   haar_wavelet <path/to/image> <output>

#include <imagestego/wavelet/haar.hpp>

#include <opencv2/imgcodecs.hpp>

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: haar_wavelet <path/to/image> <output>" << std::endl;
        return 1;
    }
    cv::Mat src = cv::imread(argv[1]);
    imagestego::HaarWavelet wavelet;
    cv::Mat transformed = wavelet.transform(src);
    cv::imwrite(argv[2], transformed);
    return 0;
}
