/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// imagestego
#include "imagestego/wavelet/haar.hpp"
// c++ headers
#include <algorithm>
#include <chrono>
#include <ios>
#include <iostream>
// opencv
#include <opencv2/core.hpp>

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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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

    std::cout << "Total speedup is: " << double(ns.count()) / ns1.count() << std::endl;
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
