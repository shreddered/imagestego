#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

#include <utils/binary_tree.hpp>
#include <utils/bitarray.hpp>

#include <map>
#include <string>


class StringElement {

}; // class StringElement

class LzwEncoder {
private:
    static void createCharTable();
    BitArray<> encodedMsg;
public:
    static constexpr maxBits = 12;
    explicit LzwEncoder() noexcept;
    explicit LzwEncoder(const std::string& str) noexcept;
    void setMessage(const std::string& str) noexcept;
}; // class LzwEncoder

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
