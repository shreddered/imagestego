/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

// imagestego
#include "imagestego/wavelet/haar.hpp"
// gtest
#include <gtest/gtest.h>
// c++ headers
#include <algorithm>
#include <chrono>
#include <ios>
#include <iostream>
// opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace chrono = std::chrono;

void test256x256() {
    cv::Mat m(256, 256, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}
void test320x320() {
    cv::Mat m(320, 320, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

void testHD() {
    cv::Mat m(720, 1280, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

void testFullHD() {
    cv::Mat m(1080, 1920, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

void test2K() {
    cv::Mat m(1440, 2560, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

void test4K() {
    cv::Mat m(2160, 3840, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

void test501x303() {
    cv::Mat m(303, 501, CV_8UC3), dst;
    cv::randu(m, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    imagestego::HaarWavelet wavelet;
    // start
    auto start = chrono::high_resolution_clock::now();
    dst = wavelet.transform(m);
    auto end = chrono::high_resolution_clock::now();
    // end
    chrono::nanoseconds ns = end - start;
    std::cout << ns.count() << " ns" << '\n';

    imagestego::experimental::HaarWavelet w2;

    start = chrono::high_resolution_clock::now();
    cv::Mat dst1 = w2.transform(m);
    end = chrono::high_resolution_clock::now();
    chrono::nanoseconds ns1 = end - start;
    std::cout << ns1.count() << " ns" << '\n';

    std::cout << "Total speedup is: " << double(ns.count())/ns1.count() << std::endl;
}

int main() {
    test256x256();
    test320x320();
    test501x303();
    testHD();
    testFullHD();
    test2K();
    test4K();
    return 0;
}
