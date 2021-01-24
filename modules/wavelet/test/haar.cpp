/*
 * this file is a part of imagestego library.
 *
 * copyright (c) 2020-2021 dmitry kalinin <x.shreddered.x@gmail.com>
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * software.
 */

// imagestego headers
#include "imagestego/wavelet/haar.hpp"
// c++ headers
#include <algorithm>
#include <iostream>
// gtest
#include <gtest/gtest.h>
// opencv headers
#include <opencv2/core.hpp>

TEST(Wavelet, Haar) {
    cv::Mat m(3, 4, CV_8UC1), dst;
    cv::randu(m, cv::Scalar(0), cv::Scalar(255));
    std::cout << m << std::endl;
    imagestego::HaarWavelet wavelet;
    dst = wavelet.transform(m);
    std::cout << dst << std::endl;
    cv::Mat tmp = wavelet.inverse(dst);
    std::cout << tmp << std::endl;
}

TEST(Wavelet, InverseVectorHaar) {
    cv::Mat m(1, 32, CV_16SC1);
    cv::randu(m, cv::Scalar(0), cv::Scalar(255));
    std::cout << "original: " << m << std::endl;
    imagestego::HaarWavelet w1;
    imagestego::experimental::HaarWavelet w2;
    cv::Mat dst = w2.transform(m);
    std::cout << dst << std::endl;
    cv::Mat exp = w2.inverse(dst),
            actual = w1.inverse(dst);
    std::cout << std::hex << "inverse(v): " << exp << std::endl
        << "inverse:    " << actual << std::endl;
}
