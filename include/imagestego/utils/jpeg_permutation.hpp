#ifndef __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/jpeg_image.hpp"
// c++ headers
#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <vector>
// third party
#include "MurmurHash3.h"


namespace imagestego {

class Permutation;

}

std::vector<short> operator *(const imagestego::Permutation& p, const imagestego::JpegImage& im);
std::vector<short> operator *(const std::vector<short>& col, const imagestego::Permutation& p);

namespace imagestego {

enum Direction : bool {
    horizontal = false,
    vertical = true
};

// Matrix permutation
class Permutation {
public:
    explicit Permutation() noexcept;
    explicit Permutation(const int& dim, bool dir);
    void generate(std::mt19937& gen);
    void inverse() const;
private:
    friend std::vector<short> (::operator *)(const imagestego::Permutation& p, const std::vector<short>& row);
    friend std::vector<short> (::operator *)(const std::vector<short>& col, const imagestego::Permutation& p);
    mutable std::vector<int> mat;
    bool dir;
}; // class Permutation

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__ */
