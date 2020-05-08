#ifndef __IMAGESTEGO_CORE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_HPP_INCLUDED__

// c++
#include <exception>
#include <string>

#if (defined(WIN32) || defined(_WIN32) || defined (WINCE))
#   define IMAGESTEGO_EXPORTS __declspec(dllexport)
#else
#   define IMAGESTEGO_EXPORTS /* nothing */
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1600)
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t
    typedef unsigned int uint32_t;
    typedef unsigned __int64 uint64_t;
#else	// defined(_MSC_VER)
#   include <cstdint>
#endif

namespace imagestego {

class IMAGESTEGO_EXPORTS Exception : public std::exception {
public:
    explicit Exception(const int& _code) noexcept;
    const char* what() const noexcept override;
    enum Codes {
        NoSuchFile = 1,
        BigMessageSize = 1 << 1,
        NoKeyFound = 1 << 2,
        InternalError = 1 << 3,
        UnknownLsbMode = 1 << 4,
        NotJpegClass = 1 << 5
    };
private:
    int code;
};

enum class Mode {
    embedding,
    extraction
};

enum class Algorithm {
    Lsb,
    JpegLsb,
    Dwt
};

class IMAGESTEGO_EXPORTS AbstractStegoEmbedder {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setOutputName(const std::string& filename) = 0;
    virtual void createStegoContainer() const = 0;
    virtual void setMessage(const std::string& msg) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual Algorithm getAlgorithm() const = 0; 
    static constexpr inline Mode getMode() noexcept {
        return Mode::embedding;
    }
    virtual ~AbstractStegoEmbedder() = default;
}; // class AbstractStegoEmbedder

class IMAGESTEGO_EXPORTS AbstractStegoExtracter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual std::string extractMessage() = 0;
    virtual Algorithm getAlgorithm() const = 0; 
    static constexpr inline Mode getMode() noexcept {
        return Mode::extraction;
    }
    virtual ~AbstractStegoExtracter() = default;
}; // class AbstractStegoExtracter

uint8_t log2(unsigned int value) noexcept;

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_HPP_INCLUDED__ */
