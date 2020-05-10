
#ifndef __LZW_DECODER_HPP_INCLUDED__
#define __LZW_DECODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/compression/lzw_dictionary.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++ headers
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS LzwDecoder : private Dictionary {
public:
    static constexpr uint8_t maxBits = 12;
    explicit LzwDecoder() noexcept;
    explicit LzwDecoder(const BitArray<>& str) noexcept;
    template<typename Int>
    void setMessage(const BitArray<Int>& arr) noexcept {
        msg = arr;
        decodedMsg.clear();
    }
    std::string getDecodedMessage();
private:
    std::string decodedMsg;
    mutable BitArray<> msg;
    void decode();
}; // class LzwDecoder

} // namespace imagestego 

#endif /* __LZW_DECODER_HPP_INCLUDED__ */
