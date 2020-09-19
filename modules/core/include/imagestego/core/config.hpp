#ifndef __IMAGESTEGO_CONFIG_HPP_INCLUDED__
#define __IMAGESTEGO_CONFIG_HPP_INCLUDED__

#if (defined(WIN32) || defined(_WIN32) || defined (WINCE))
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

#endif /* __IMAGESTEGO_CONFIG_HPP_INCLUDED__ */
