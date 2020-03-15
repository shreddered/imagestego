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
        for (uint8_t i = 0; i < sizeof(Block); ++i)
            tmp |= static_cast<uint8_t>(str[pos + i]) << ((sizeof(Block) - i - 1) * CHAR_BIT);
        return tmp;
    }
    static Block __block_last(const std::string& str, const std::size_t& pos) noexcept {
        Block tmp = 0;
        for (uint8_t i = 0; i + pos < str.size(); ++i)
            tmp |= static_cast<uint8_t>(str[pos + i]) << ((sizeof(Block) - i - 1) * CHAR_BIT);
        return tmp;
    }
    class BitReference {
        friend class BitArray<Block>;
    private:
        Block& block;
        Block mask;
        explicit BitReference(Block& b, std::size_t pos) : block(b), mask(1 << (bitsPerBlock - pos)) {}
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
    std::size_t numberOfBits;
    std::vector<Block> array; 
public:
    explicit BitArray(const std::string& str) noexcept : numberOfBits(str.length() * 8),
    array(numberOfBlocks(numberOfBits)) {
        for (std::size_t i = 0; i < str.size(); i += sizeof(Block)) {
            if (str.size() - i < sizeof(Block)) // last block case
                array[i % sizeof(Block)] = __block_last(str, i);
            else
                array[i % sizeof(Block)] = __block(str, i);
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
        std::string result(BitArray<char>::numberOfBlocks);
        return result;
    }
}; // class BitArray

#endif /* __BITARRAY_HPP_INCLUDED__ */
