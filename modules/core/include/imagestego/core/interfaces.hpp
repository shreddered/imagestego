#ifndef __IMAGESTEGO_INTERFACES_HPP_INCLUDED__
#define __IMAGESTEGO_INTERFACES_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/config.hpp"
// c++
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

class IMAGESTEGO_EXPORTS AbstractEncoder {
public:
    virtual void setMessage(const std::string& msg) = 0;
    virtual BitArray getEncodedMessage() = 0; 
}; // class AbstractEncoder

class IMAGESTEGO_EXPORTS AbstractDecoder {
public:
    virtual void setMessage(const BitArray& arr) = 0;
    virtual std::string getDecodedMessage() = 0;
}; // class AbstractDecoder

} // namespace imagestego

#endif /* __IMAGESTEGO_INTERFACES_HPP_INCLUDED__ */
