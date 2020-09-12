#ifndef __IMAGESTEGO_BITARRAY_HPP_INCLUDED__
#define __IMAGESTEGO_BITARRAY_HPP_INCLUDED__

// imagestego
#include "imagestego/core.hpp"
// c++ headers
#include <algorithm>
#include <cstring>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#ifndef CHAR_BIT
#   define CHAR_BIT 8
#endif


namespace imagestego {

class HuffmanDecoder;

template<class Block = uint16_t, class = typename std::enable_if<std::is_integral<Block>::value>::type>
class IMAGESTEGO_EXPORTS BitArray {
    friend class HuffmanDecoder;
private:
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
    class Iterator {
        BitArray<Block>* owner;
        std::size_t index;
        friend class BitArray<Block>;
        explicit Iterator(BitArray<Block>* _owner, const std::size_t& idx) noexcept 
            : owner(_owner), index(idx) {}
    public:
        BitReference operator *() noexcept {
            return (*owner)[index];
        }
        bool operator == (const Iterator& it) noexcept {
            return index == it.index;
        }
        bool operator != (const Iterator& it) noexcept {
            return index != it.index;
        }
        Iterator operator ++() {
            ++index;
            return *this;
        }
        Iterator operator +(std::size_t offset) noexcept {
            index += offset;
            return *this;
        }
    }; // class Iterator
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
    typedef Iterator iterator;
    enum {
        bitString = 0,
        byteString = 1
    };
    explicit BitArray() noexcept {}
    explicit BitArray(const std::size_t size) noexcept : numberOfBits(size), array(numberOfBlocks(size)) {}
    explicit BitArray(const std::string& str, int mode = byteString) noexcept 
        : numberOfBits((mode) ? str.length() * 8 : str.length()), array(numberOfBlocks(numberOfBits)) {
        if (mode == 1) {
            std::string tmp = str + std::string(str.size() % sizeof(Block), '\0');
            for (int i = 0; i != tmp.size() / sizeof(Block); ++i)
                std::reverse(tmp.begin() + i * sizeof(Block), tmp.begin() + i * sizeof(Block) + sizeof(Block));
            memcpy(&array[0], tmp.data(), tmp.size()); 
        }
        else {
            // TODO: improve it 
            for (std::size_t i = 0; i != str.size(); ++i)
                operator [](i) = str[i] - '0';
        }
    }
    template<typename Int>
    BitArray<Block>& operator =(const BitArray<Int>& other) {
        clear();
        if (reinterpret_cast<const char*>(this) != reinterpret_cast<const char*>(&other)) {
            for (std::size_t i = 0; i != other.size(); ++i)
                pushBack(other[i]);
        }
        return *this;
    }
    template<typename Int> 
    static typename std::enable_if<(sizeof(Block) != sizeof(Int) && std::is_integral<Int>::value), 
                    BitArray<Block> >::type fromInt(const Int& num) {
        BitArray<Block> arr;
        arr.pushBack(num, sizeof(Int) * CHAR_BIT);
        return arr;
    }
    template<typename Int>
    static typename std::enable_if<(sizeof(Block) == sizeof(Int) && std::is_integral<Int>::value), 
                    BitArray<Block> >::type fromInt(const Int& num) {
        BitArray<Block> arr;
        arr.array.push_back(num);
        arr.numberOfBits = sizeof(Int) * CHAR_BIT;
        return arr;
    }
    BitReference operator [](std::size_t pos) noexcept {
        return BitReference(array[blockIndex(pos)], bitIndex(pos));
    }
    bool operator [](std::size_t pos) const noexcept {
        return array[blockIndex(pos)] & (1 << (bitsPerBlock - bitIndex(pos) - 1));
    }
    inline Block getBlock(std::size_t pos) const noexcept {
        return array[pos];
    }
    inline std::size_t size() const noexcept {
        return numberOfBits;
    }
    std::string toString() const noexcept {
        std::string result;
        for (std::size_t i = 0; i < array.size();++i) {
            const char* ptr = reinterpret_cast<const char*>(&array[i]);
            auto __tmp = std::string(ptr, sizeof(Block));
            result += std::string(__tmp.rbegin(), __tmp.rend());
        }
        return result;
    }
    void clear() noexcept {
        array.clear();
        numberOfBits = 0;
    }
    bool empty() const noexcept {
        return array.empty();
    }
    BitReference back() noexcept {
        return operator[](size() - 1);
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
    void pushBack(std::size_t num, uint8_t bits) {
        for (uint8_t i = 0; i != bits; ++i) {
            const uint8_t offset = bits - 1 - i;
            pushBack((num & (1 << offset)) != 0);
        }
    }
    inline Block lastBlock() const noexcept {
        return array[array.size() - 1];
    }
    iterator begin() noexcept {
        return Iterator(this, 0);
    }
    iterator end() noexcept {
        return Iterator(this, size());
    }
}; // class BitArray

// xoring with random values
template<typename T>
void randomize(BitArray<T>& arr, std::mt19937& gen) { 
    for (std::size_t i = 0; i != arr.size(); ++i)
        arr[i] = (gen() % 2 != 0) != arr[i];
}

} // namespace imagestego

template<class Block>
std::ostream& operator <<(std::ostream& out, const imagestego::BitArray<Block>& arr) {
    for (std::size_t i = 0; i != arr.size(); ++i)
        out << arr[i];
    return out;
}

template<typename T1, typename T2, typename T = typename std::common_type<T1, T2>::type>
imagestego::BitArray<T> operator +(const imagestego::BitArray<T1>& arr1, const imagestego::BitArray<T2>& arr2) {
    imagestego::BitArray<T> arr(arr1.size() + arr2.size());
    for (std::size_t i = 0; i != arr1.size(); ++i)
        arr[i] = arr1[i];
    for (std::size_t i = 0; i != arr2.size(); ++i)
        arr[i + arr1.size()] = arr2[i];
    return arr;
}

template<typename T1, typename T2, typename T = typename std::common_type<T1, T2>::type>
imagestego::BitArray<T> operator ^(const imagestego::BitArray<T1>& arr1, const imagestego::BitArray<T2>& arr2) {
    if (arr1.size() != arr2.size())
        throw std::runtime_error("^ is defined only for bit arrays of same size");
    imagestego::BitArray<T> arr(arr1.size());
    for (std::size_t i = 0; i != arr1.size(); ++i) {
        arr[i] = arr1[i] != arr2[i];
    }
    return arr;
}

#endif /* __IMAGESTEGO_BITARRAY_HPP_INCLUDED__ */
