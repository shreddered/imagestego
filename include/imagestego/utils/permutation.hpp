#ifndef __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__
#define __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__

// imagestego  headers
#include "imagestego/core.hpp"
#include "imagestego/utils/jpeg_processor.hpp"
// c++ headers
#include <random>
#include <string>
// third party
#include "MurmurHash3.h"


namespace imagestego {

class JpegPermutation : protected JpegProcessor {
public:
    virtual ~JpegPermutation() noexcept;
protected:
    explicit JpegPermutation() noexcept;
    explicit JpegPermutation(const std::string& src);
    void seed(const std::string& key);
    void process() const override;
private:
    mutable std::mt19937 gen;
    uint32_t key;
}; // class JpegPermutation

} // namespace imagestego

#endif /* __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__ */
