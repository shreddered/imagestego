#ifndef __IMAGESTEGO_INTRINSIC_HPP_INCLUDED__
#define __IMAGESTEGO_INTRINSIC_HPP_INCLUDED__

#include "imagestego/core/config.hpp"


namespace imagestego {

IMAGESTEGO_EXPORTS uint8_t log2(uint32_t value) noexcept;

IMAGESTEGO_EXPORTS uint32_t bswap(uint32_t) noexcept;

} // namespace imagestego

#endif /* __IMAGESTEGO_INTRINSIC_HPP_INCLUDED__ */
