/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef __IMAGESTEGO_CONFIG_HPP_INCLUDED__
#define __IMAGESTEGO_CONFIG_HPP_INCLUDED__

#if (defined(WIN32) || defined(_WIN32) || defined (WINCE)) && defined(IMAGESTEGO_DLL)
#   ifdef IMAGESTEGO_EXPORT
#       define IMAGESTEGO_EXPORTS __declspec(dllexport)
#   else
#       define IMAGESTEGO_EXPORTS __declspec(dllimport)
#   endif
#else
#   define IMAGESTEGO_EXPORTS /* nothing */
#endif

#define IMAGESTEGO_MURMURHASH_SEED 4991

#if defined(_MSC_VER) && (_MSC_VER < 1600)
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t
    typedef unsigned int uint32_t;
    typedef unsigned __int64 uint64_t;
#else
#   include <cstdint>
#endif

namespace imagestego {

/**
 * Type definition provided for convenient usage.
 */
typedef uint32_t size_t;

} // namespace imagestego

#endif /* __IMAGESTEGO_CONFIG_HPP_INCLUDED__ */
