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

class Permutation;

}

std::vector<short> operator *(const imagestego::Permutation& p, const std::vector<short>& row);
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

class JpegPermutation : protected JpegProcessor {
public:
    virtual ~JpegPermutation() noexcept = default;
protected:
    explicit JpegPermutation(const std::string& src);
    void generate(const std::string& key);
    void process() const override;
private:
    Permutation p1, p2;
    std::mt19937 gen;
    std::vector<short> getRow(const int& row, const int& channel = 0) const;
    void setRow(const int& row, const std::vector<short>& arr, const int& channel = 0) const; 
    std::vector<short> getCol(const int& row, const int& channel = 0) const;
    void setCol(const int& col, const std::vector<short>& arr, const int& channel = 0) const; 
}; // class JpegPermutation

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_PERMUTATON_HPP_INCLUDED__ */
