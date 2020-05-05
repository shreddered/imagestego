#ifndef __IMAGESTEGO_HPP_INCLUDED__
#define __IMAGESTEGO_HPP_INCLUDED__

// imagestego headers
#include "imagestego/algorithms/dwt.hpp"
#include "imagestego/algorithms/lsb.hpp"
#include "imagestego/algorithms/jpeg_lsb.hpp"

#include "imagestego/compression/huffman_decoder.hpp"
#include "imagestego/compression/huffman_encoder.hpp"

#include "imagestego/core.hpp"
// c++ headers
#include <memory>
#include <string>


namespace imagestego {

void embedSecretMessage(AbstractStegoEmbedder* embedder, const std::string& image,
        const std::string& output, const std::string& message, const std::string& key);

std::string extractSecretMessage(AbstractStegoExtracter* extracter, const std::string& image, const std::string& key);

} // namespace imagestego

#endif /* __IMAGESTEGO_HPP_INCLUDED__ */
