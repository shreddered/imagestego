#ifndef __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/jpeg_processor.hpp"
// c++ headers
#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>
// third party
#include "MurmurHash3.h"


namespace imagestego {

// Matrix permutation
class Permutation {
public:
    explicit Permutation() noexcept;
    explicit Permutation(const int& rows, const int& cols);
    void seed(const std::string& key);
    Permutation inverse();
private:
    static void generate(std::vector<int>& v);
    std::vector<int> left;
    std::vector<int> right;
    std::mt19937 gen;
}; // class Permutation

class JpegPermutation : public Permutation {
}; // class JpegPermutation

} // namespace

#endif /* __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__ */
