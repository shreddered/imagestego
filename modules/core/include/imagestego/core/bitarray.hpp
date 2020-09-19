#ifndef __IMAGESTEGO_BITARRAY_HPP_INCLUDED__
#define __IMAGESTEGO_BITARRAY_HPP_INCLUDED__

// c++
#include <ostream>
#include <string>
#include <vector>

#ifndef CHAR_BIT
#   define CHAR_BIT 8
#endif


namespace imagestego {

class BitArray;

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr);

namespace imagestego {

class BitArrayImpl final {
    class BitReference;
    class BitIterator;
public:
    typedef BitReference reference;
    typedef BitIterator iterator;
    typedef const BitIterator const_iterator;
    explicit BitArrayImpl() noexcept;
    BitArrayImpl(const std::string& str);
    explicit BitArrayImpl(std::size_t sz);
    static BitArrayImpl fromByteString(std::string s);
    std::size_t size() const noexcept;
    BitReference operator [](std::size_t i);
    bool operator [](std::size_t i) const;
    void pushBack(bool val);
    iterator begin();
    iterator end();
private:
    typedef uint32_t BlockType;
    class BitReference final {
    private:
        friend class BitArrayImpl;
        friend class BitIterator;
        BlockType& _block;
        BlockType _mask;
        explicit BitReference(BlockType& block, std::size_t pos) noexcept;
    public:
        operator bool() const;
        BitReference& operator =(bool val) noexcept;
        BitReference& operator =(const BitReference& other) noexcept;
        bool operator ==(const BitReference& other) noexcept;
    }; // class BitReference
    class BitIterator final {
    private:
        friend class BitArrayImpl;
        BitReference _ref;
        explicit BitIterator(BitReference ref) noexcept;
    public:
        BitReference operator *();
        BitIterator& operator ++();
        BitIterator operator ++(int);
        BitIterator& operator --();
        BitIterator operator --(int);
        BitIterator& operator +=(std::size_t i);
        BitIterator& operator -=(std::size_t i);
        BitIterator operator +(std::size_t i);
        BitIterator operator -(std::size_t i);
        bool operator ==(const BitIterator& other) noexcept;
        bool operator !=(const BitIterator& other) noexcept;
    }; // class BitIterator
    std::vector<BlockType> _blocks;
    std::size_t _sz;
    static constexpr std::size_t bitsPerBlock = CHAR_BIT * sizeof(BlockType);
    inline static constexpr std::size_t numberOfBlocks(std::size_t sz) {
        return sz / bitsPerBlock + ((sz % bitsPerBlock != 0) ? 1 : 0);
    }
    inline static constexpr std::size_t blockIndex(std::size_t i) {
        return i / bitsPerBlock;
    }
    inline static constexpr std::size_t bitIndex(std::size_t i) {
        return i % bitsPerBlock;
    }
}; // class BitArrayImpl

class BitArray {
private:
    BitArrayImpl* _arr;
public:
    typedef typename BitArrayImpl::iterator iterator;
    explicit BitArray();
    BitArray(const std::string& str);
    explicit BitArray(std::size_t sz);
    BitArray(const BitArray& other);
    BitArray& operator =(const BitArray& other);
    BitArray(BitArray&& other) noexcept;
    BitArray& operator =(BitArray&& other) noexcept;
    virtual ~BitArray() noexcept;
    static BitArray fromByteString(std::string str);
    typename BitArrayImpl::reference operator[](std::size_t i);
    std::size_t size() const noexcept;
    bool operator[](std::size_t i) const;
    void pushBack(bool val);
    friend std::ostream& (::operator <<)(std::ostream& os, const imagestego::BitArray& arr);
    iterator begin();
    iterator end();
}; // class BitArray

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArrayImpl& arr);

#endif /* __IMAGESTEGO_BITARRAY_HPP_INCLUDED__ */
