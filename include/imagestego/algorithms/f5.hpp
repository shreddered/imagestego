#ifndef __IMAGESTEGO_F5_HPP_INCLUDED__
#define __IMAGESTEGO_F5_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagesteo/utils/jpeg_processor.hpp"
// c++ headers
#include <string>


namespace imagestego {

class F5Embedder : public AbstractStegoEmbedder, private JpegProcessor {
public:
    explicit F5Embedder() noexcept;
    explicit F5Embedder(const std::string& imagename, const std::string& output);
private:
    std::string& outputFile;
    uint32_t key;
}; // class F5Embedder

}; //namespace imagestego

#endif /* __IMAGESTEGO_F5_HPP_INCLUDED__ */
