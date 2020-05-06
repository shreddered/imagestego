#ifndef __IMAGESTEGO_KEYGEN_HPP_INCLUDED__
#define __IMAGESTEGO_KEYGEN_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <cstdint>
#include <fstream>
#include <random>
#include <string>


namespace imagestego {

namespace keygen {

IMAGESTEGO_EXPORTS std::string generate(const uint32_t& len = 16);

}

} // namespace imagestego

#endif /* __IMAGESTEGO_KEYGEN_HPP_INCLUDED__ */
