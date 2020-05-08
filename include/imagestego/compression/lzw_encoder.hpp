#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++ headers
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS LzwEncoder : private Dictionary {
public:
    explicit LzwEncoder() noexcept;
    explicit LzwEncoder(const std::string& str) noexcept;
    explicit LzwEncoder(std::string&& str) noexcept;
    void setMessage(const std::string& str) noexcept;
    void setMessage(std::string&& str) noexcept;
    BitArray<> getEncodedMessage();
private:
    static constexpr std::size_t maxDictionarySize = (1 << maxBits) - 1; 
    std::string msg;
    mutable BitArray<> encodedMsg;
    void encode();
}; // class LzwEncoder

} // namespace imagestego 

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
