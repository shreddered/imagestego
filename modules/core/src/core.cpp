#include "imagestego/core.hpp"
// third party
#include "MurmurHash3.h"


namespace imagestego {

uint32_t hash(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), IMAGESTEGO_MURMURHASH_SEED, tmp);
    return tmp[0];
}

} // namespace imagestego
