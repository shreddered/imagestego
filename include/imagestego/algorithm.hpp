#ifndef __IMAGESTEGO_ALGORITHM_HPP_INCLUDED__
#define __IMAGESTEGO_ALGORITHM_HPP_INCLUDED__

#ifdef IMAGESTEGO_LOSSLESS_FORMATS
#include "imagestego/algorithms/dwt.hpp"
#include "imagestego/algorithms/lsb.hpp"
#endif /* IMAGESTEGO_LOSSLESS_FORMATS */

#ifdef IMAGESTEGO_JPEG_SUPPORT
#include "imagestego/algorithms/f3.hpp"
#include "imagestego/algorithms/jpeg_lsb.hpp"
#endif /* IMAGESTEGO_JPEG_SUPPORT */

#endif /* __IMAGESTEGO_ALGORITHM_HPP_INCLUDED__ */
