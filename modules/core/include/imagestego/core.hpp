#ifndef __IMAGESTEGO_CORE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
// c++
#include <exception>
#include <string>


namespace imagestego {

enum class Mode {
    embedding,
    extraction
};

enum class Algorithm {
    Lsb,
    JpegLsb,
    Dwt,
    F3,
    UserDefined
};

class IMAGESTEGO_EXPORTS AbstractStegoEmbedder {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setOutputName(const std::string& filename) = 0;
    virtual void setMessage(const std::string& msg) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual Algorithm getAlgorithm() const {
        return Algorithm::UserDefined;
    } 
    virtual void createStegoContainer() = 0;
    static constexpr inline Mode getMode() noexcept {
        return Mode::embedding;
    }
    virtual ~AbstractStegoEmbedder() = default;
}; // class AbstractStegoEmbedder

class IMAGESTEGO_EXPORTS AbstractStegoExtracter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual Algorithm getAlgorithm() const {
        return Algorithm::UserDefined;
    }
    virtual std::string extractMessage() = 0;
    static constexpr inline Mode getMode() noexcept {
        return Mode::extraction;
    }
    virtual ~AbstractStegoExtracter() = default;
}; // class AbstractStegoExtracter

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
