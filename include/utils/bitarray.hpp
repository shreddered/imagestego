#ifndef __BITARRAY_HPP_INCLUDED__
#define __BITARRAY_HPP_INCLUDED__

#include <cstddef>
#include <string>
#include <vector>

#ifndef CHAR_BIT
#   define CHAR_BIT 8
#endif


template<class Block = uint16_t>
class BitArray {
private:
    static Block __block(const std::string& str, const std::size_t& pos) noexcept {
        Block tmp = 0;
        for (uint8_t i = 0; i < sizeof(Block) && i + pos < str.size(); ++i)
            tmp |= static_cast<uint8_t>(str[pos + i]) << ((sizeof(Block) - i - 1) * CHAR_BIT);
        return tmp;
    }
    class BitReference {
        friend class BitArray<Block>;
        Block& block;
        Block mask;
        explicit BitReference(Block& b, std::size_t pos) : block(b), mask(1 << (bitsPerBlock - pos - 1)) {}
    public:
#if __cplusplus >= 201103L
        BitReference(const BitReference& other) noexcept = default;
#endif
        operator bool() const {
            return (block & mask) != 0;
        }
        BitReference& operator =(bool val) noexcept {
            if (val)
                block |= mask; // always true on access (* | 1 = 1)
            else
                block &= ~mask; // always false on access (* & 0 = 0)
            return *this;
        }
        BitReference& operator =(const BitReference& other) noexcept {
            if (other)
                block |= mask;
            else
                block &= ~mask;
            return *this;
        }
    }; // class BitReference
    static constexpr std::size_t bitsPerBlock = CHAR_BIT * sizeof(Block); 
    inline static constexpr std::size_t bitIndex(std::size_t pos) noexcept {
        return pos % bitsPerBlock;
    }
    inline static constexpr std::size_t blockIndex(std::size_t pos) noexcept {
        return pos / bitsPerBlock;
    }
    inline static constexpr std::size_t numberOfBlocks(const std::size_t& size) noexcept {
        return size / bitsPerBlock + static_cast<std::size_t>(size % bitsPerBlock != 0);
    }
    std::size_t numberOfBits = 0;
    std::vector<Block> array; 
public:
    explicit BitArray() noexcept {}
    explicit BitArray(const std::string& str) noexcept : numberOfBits(str.length() * 8),
    array(numberOfBlocks(numberOfBits)) {
        for (std::size_t i = 0; i < str.size(); i += sizeof(Block)) {
            array[i / sizeof(Block)] = __block(str, i);
        }
    }
    BitReference operator [](std::size_t pos) noexcept {
        return BitReference(array[blockIndex(pos)], bitIndex(pos));
    }
#ifdef _DEBUG
    inline Block getBlock(std::size_t pos) const noexcept {
        return array[pos];
    }
#endif
    inline std::size_t size() const noexcept {
        return numberOfBits;
    }
    std::string toString() const noexcept {
        std::string result;
        //result.reserve(BitArray<char>::numberOfBlocks(numberOfBits));
        for (std::size_t i = 0; i < array.size();++i) {
            const char* ptr = reinterpret_cast<const char*>(&array[i]);
            auto __tmp = std::string(ptr, sizeof(Block));
            result += std::string(__tmp.rbegin(), __tmp.rend());
        }
        return result;
    }
    void put(unsigned char c) noexcept { // needed
        if (array.empty())
            array.push_back(0);
        const auto estimatedSpace = array.size() * bitsPerBlock - numberOfBits;
        if (estimatedSpace < CHAR_BIT) {
            const auto unwritten = CHAR_BIT - estimatedSpace;
            array[array.size() - 1] |= c >> (unwritten);
            array.push_back(0);
            array[array.size() - 1] |= (c & ((1 <<  unwritten) - 1)) << (bitsPerBlock - unwritten); 
        }
        else
            array[array.size() - 1] |= c << (estimatedSpace - CHAR_BIT); 
        numberOfBits += 8;
    }
    void pushBack(bool b) noexcept {
        if (numberOfBits++ == array.size() * bitsPerBlock)
            array.push_back(0);
        auto tmp = BitReference(array[blockIndex(numberOfBits - 1)], bitIndex(numberOfBits - 1));
        tmp = b;
    }
    inline Block lastBlock() const noexcept {
        return array[array.size() - 1];
    }
}; // class BitArray

template<class Block>
std::ostream& operator <<(std::ostream& out, BitArray<Block> arr) {
    for (std::size_t i = 0; i != arr.size(); ++i)
        out << arr[i];
    return out;
}

#endif /* __BITARRAY_HPP_INCLUDED__ */
