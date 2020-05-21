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
#include <vector>
// third party
#include "MurmurHash3.h"


namespace imagestego {

enum Direction : bool {
    horizontal = false,
    vertical = true
};

// Matrix permutation
class Permutation {
public:
    explicit Permutation() noexcept;
    explicit Permutation(const int& dim);
    void seed(const std::string& key);
    void inverse();
private:
    void generate(std::vector<int>& v);
    std::vector<int> mat;
    std::mt19937 gen;
    bool dir;
}; // class Permutation

class JpegPermutation : public Permutation {
}; // class JpegPermutation

} // namespace

#endif /* __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__ */
