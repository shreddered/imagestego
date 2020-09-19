#ifndef __IMAGESTEGO_CORE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/avl_tree.hpp"
//#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/config.hpp"
#include "imagestego/core/exception.hpp"
#include "imagestego/core/interfaces.hpp"
#include "imagestego/core/intrinsic.hpp"


namespace imagestego {

template<class It, class Rng>
void shuffle(It first, It last, Rng&& gen) {
    for (auto it = first; it != last; ++it) {
        auto tmp = gen() % (it - first + 1);
        std::swap(*(first + tmp), *it);
    }
}

IMAGESTEGO_EXPORTS uint32_t hash(const std::string& _key);

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_HPP_INCLUDED__ */
