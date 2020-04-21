#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

#include<utils/bitarray.hpp>

#include <map>
#include <string>


class LzwEncoder {
private:
    std::map<std::string, std::int> codeTable;
    static void createCharTable();
    BitArray<> encodedMsg;
public:
    explicit LzwEncoder() noexcept;
    explicit LzwEncoder(const std::string& str) noexcept;

}; // class LzwEncoder

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
