#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"
#include "imagestego/utils/binary_tree.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++ headers
#include <map>
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS LzwEncoder : private Dictionary {
public:
    static constexpr uint8_t maxBits = 12;
    explicit LzwEncoder() noexcept;
    explicit LzwEncoder(const std::string& str) noexcept;
    explicit LzwEncoder(std::string&& str) noexcept;
    void setMessage(const std::string& str) noexcept;
    void setMessage(std::string&& str) noexcept;
    BitArray<> getEncodedMessage();
private:
    std::string msg;
    mutable BitArray<> encodedMsg;
    void encode();
}; // class LzwEncoder

} // namespace imagestego 

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
