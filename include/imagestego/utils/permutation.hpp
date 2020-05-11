#ifndef __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__
#define __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__

// imagestego  headers
#include "imagestego/core.hpp"
#include "imagestego/utils/jpeg_processor.hpp"
// c++ headers
#include <random>
#include <string>


namespace imagestego {

class JpegPermutation : protected JpegProcessor {
public:
    virtual ~JpegPermutation() noexcept;
protected:
    explicit JpegPermutation() noexcept;
    explicit JpegPermutation(const std::string& src);
    void read(const std::string& src);
    void process() const override;
private:
    std::mt19937 gen;
}; // class JpegPermutation

} // namespace imagestego

#endif /* __IMAGESTEGO_PERMUTATION_HPP_INCLUDED__ */
