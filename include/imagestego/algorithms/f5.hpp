#ifndef __IMAGESTEGO_F5_HPP_INCLUDED__
#define __IMAGESTEGO_F5_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagesteo/utils/jpeg_permutation.hpp"
// c++ headers
#include <string>


namespace imagestego {

class F5Embedder : public AbstractStegoEmbedder, private JpegPermutation {
public:
    explicit F5Embedder() noexcept;
    explicit F5Embedder(const std::string& imagename, const std::string& output);
private:
    std::string& outputFile;
    BitArray<> msg;
}; // class F5Embedder

}; //namespace imagestego

#endif /* __IMAGESTEGO_F5_HPP_INCLUDED__ */
