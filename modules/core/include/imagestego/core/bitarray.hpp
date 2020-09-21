/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef __IMAGESTEGO_BITARRAY_HPP_INCLUDED__
#define __IMAGESTEGO_BITARRAY_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
// c++ headers
#include <ostream>
#include <string>
#include <vector>

#ifndef CHAR_BIT
#   define CHAR_BIT 8
#endif

// forward declarations (needed for operator <<)
namespace imagestego {

class BitArray;

} // namespace imagestego

/** 
 * Operator << overloading.
 *
 * Operator << overloading for printing bit array.
 * 
 * @param os std::ostream instace.
 * @param arr Bit array to be printed.
 * @return std::ostream instance.
 */
IMAGESTEGO_EXPORTS std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr);

namespace imagestego {

/**
 * Implementation of class BitArray for working with bit sequences.
 *
 * Implementation of this class is similar to std::vector<bool>. Also several methods 
 * provided for convenient usage with other classes.
 */
class BitArrayImpl final {
    class BitReference;
    class BitIterator;
public:
    typedef BitReference reference;
    typedef BitIterator iterator;
    typedef const BitIterator const_iterator;

    /**
     * Constructor of BitArrayImpl.
     *
     * Constructs empty array.
     */
    explicit BitArrayImpl() noexcept;

    /**
     * Constructor of BitArrayImpl.
     *
     * Constructs array from binary string.
     *
     * @param str String containing '1' and '0'.
     */
    BitArrayImpl(const std::string& str);

    /**
     * Constructor of BitArrayImpl.
     *
     * Constructs array with given size.
     *
     * @param sz Size of array.
     */
    explicit BitArrayImpl(size_t sz);

    /**
     * Converter from byte sequence to bit array.
     *
     * @param str Byte string to be converted to bit array.
     * @return Converted array.
     */
    static BitArrayImpl fromByteString(std::string str);

    /**
     * Converter from bit array to byte string.
     *
     * @return Converted byte string.
     */
    std::string toByteString() const;

    /**
     * Conventer from bit array to binary string.
     *
     * @return String representation of array containing '1' and '0'.
     */
    std::string toString() const;

    /**
     * Size of array.
     *
     * @return Size of array
     */
    size_t size() const noexcept;

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Instance of proxy class representing element.
     */
    BitReference operator [](size_t i);

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Read-only element of array.
     */
    bool operator [](size_t i) const;

    /**
     * Adds element to the end.
     *
     * @param val Value to be added
     */
    void pushBack(bool val);

    /**
     * Puts n bits from number to array.
     *
     * This method has several overloads for different integer types.
     *
     * @param num Number from which bits are taken.
     * @param n Nubmer of bits to take from num.
     */
    void put(int num, size_t n);
    void put(size_t num, size_t n);

    /**
     * Clears array.
     *
     * Sets empty() value to true.
     */
    void clear();

    /** 
     * Checks if array is empty.
     *
     * @return true if array is empty, false otherwise.
     */
    bool empty() const noexcept;

    /**
     * Iterator to beginning. 
     *
     * @return Iterator pointing to the first element of array.
     */
    iterator begin();

    /**
     * Iterator to end. 
     *
     * @return Iterator pointing to past-the-end element in array.
     */
    iterator end();

private:

    /**
     * Block type of undelying array. Integer blocks used to store bits.
     */
    typedef uint32_t BlockType;

    /**
     * Proxy class for read-write access on elements.
     */
    class IMAGESTEGO_EXPORTS BitReference final {
    private:
        friend class BitArrayImpl;
        friend class BitIterator;

        /* Integer block */
        BlockType& _block;

        /* Mask for acessing bit */
        BlockType _mask;

        /**
         * Constructor of reference type.
         *
         * @param block Block reference containing element.
         * @param pos Bit index of block.
         */
        explicit BitReference(BlockType& block, size_t pos) noexcept;
    public:

        /**
         * Cast reference to bool.
         *
         * @return Stored value.
         */
        operator bool() const;

        /**
         * Operator = overloading.
         *
         * @param val Value to be assigned.
         * @return This instance's reference.
         */
        BitReference& operator =(bool val) noexcept;

        /**
         * Operator = overloading.
         *
         * @param val Reference to be assigned.
         * @return This instance's reference.
         */
        BitReference& operator =(const BitReference& other) noexcept;

        /**
         * Comparator for two references.
         *
         * @return true if references are equal, false otherwise.
         */
        bool operator ==(const BitReference& other) noexcept;
    }; // class BitReference

    /**
     * Bit array iterator class.
     */
    class BitIterator final {
    private:
        friend class BitArrayImpl;

        /** Reference iterator points to. */
        BitReference _ref;

        /**
         * Constructs iterator.
         *
         * @param ref Reference iterator points to.
         */
        explicit BitIterator(BitReference ref) noexcept;
    public:

        /**
         * Unary * overloading.
         *
         * @return Reference iterator points to.
         */
        BitReference operator *();

        /**
         * Prefix increment overloading.
         *
         * @return This instance's reference.
         */
        BitIterator& operator ++();

        /**
         * Postfix increment overloading.
         *
         * @return Iterator befor it was incremented.
         */
        BitIterator operator ++(int);

        /**
         * Prefix decrement overloading.
         *
         * @return This instance's reference.
         */
        BitIterator& operator --();

        /**
         * Postfix decrement overloading.
         *
         * @return Iterator befor it was decremented.
         */
        BitIterator operator --(int);

        // BitIterator& operator +=(size_t i);

        // BitIterator& operator -=(size_t i);

        // BitIterator operator +(size_t i);

        // BitIterator operator -(size_t i);

        /**
         * Iterator comparison.
         */
        bool operator ==(const BitIterator& other) noexcept;
        bool operator !=(const BitIterator& other) noexcept;
    }; // class BitIterator

    /** Array of integer blocks. */
    std::vector<BlockType> _blocks;

    /** Size of array. */
    size_t _sz;

    /** Number of bits per block. */
    static constexpr size_t bitsPerBlock = CHAR_BIT * sizeof(BlockType);

    /**
     * Number of blocks needed to store sz bits.
     *
     * @param sz Number of bits.
     * @return Number of blocks needed to store sz bits.
     */
    inline static constexpr size_t numberOfBlocks(size_t sz) {
        return sz / bitsPerBlock + ((sz % bitsPerBlock != 0) ? 1 : 0);
    }

    /**
     * Block index of i-th element.
     *
     * @param i Index of an element.
     * @return Index of block containing i-th element.
     */
    inline static constexpr size_t blockIndex(size_t i) {
        return i / bitsPerBlock;
    }

    /**
     * Bit index of i-th element.
     *
     * @param i index of an element.
     * @return Index of bit in the block with i-th element.
     */
    inline static constexpr size_t bitIndex(size_t i) {
        return i % bitsPerBlock;
    }
}; // class BitArrayImpl

/**
 * Class representing bit array.
 *
 * This class uses PImpl technique.
 */
class IMAGESTEGO_EXPORTS BitArray {
private:

    /** Pointer to implementation. */
    BitArrayImpl* _arr;
public:

    /**
     * Iterator type definition.
     */
    typedef typename BitArrayImpl::iterator iterator;

    /**
     * Bit array constructor.
     *
     * Constructs an empty array.
     */
    explicit BitArray();

    /**
     * Bit Array constructor from binary string.
     *
     * @param str Binary string containing '1' and '0'.
     */
    BitArray(const std::string& str);

    /**
     * Constructs bit array of given length.
     *
     * @param sz Size of array.
     */
    explicit BitArray(size_t sz);

    /**
     * imagestego::BitArray copy constructor.
     */
    BitArray(const BitArray& other);

    /**
     * imagestego::BitArray copy-assignment operator overloading.
     */
    BitArray& operator =(const BitArray& other);

    /**
     * imagestego::BitArray move constructor.
     */
    BitArray(BitArray&& other) noexcept;

    /**
     * imagestego::BitArray move-assignment operator overloading.
     */
    BitArray& operator =(BitArray&& other) noexcept;

    /**
     * imagestego::BitArray destructor.
     */
    virtual ~BitArray() noexcept;

    /**
     * Converts byte array to bit array in little endian format.
     *
     * @param str Byte array to be converted to bit sequence.
     * @return Bit array converted from str.
     */
    static BitArray fromByteString(std::string str);

    /**
     * Converter from bit array to byte string.
     *
     * @return Converted byte string.
     */
    std::string toByteString() const;

    /**
     * Conventer from bit array to binary string.
     *
     * @return String representation of array containing '1' and '0'.
     */
    std::string toString() const;

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Instance of proxy class representing element.
     */
    typename BitArrayImpl::reference operator[](size_t i);

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Read-only element of array.
     */
    bool operator [](size_t i) const;

    /**
     * Size of array.
     *
     * @return Size of array
     */
    size_t size() const noexcept;

    /**
     * Puts n bits from number to array.
     *
     * This method has several overloads for different integer types.
     *
     * @param num Number from which bits are taken.
     * @param n Nubmer of bits to take from num.
     */
    void put(int num, size_t n);
    void put(size_t num, size_t n);

    /**
     * Adds element to the end.
     *
     * @param val Value to be added
     */
    void pushBack(bool val);

    /**
     * Clears array.
     *
     * Sets empty() value to true.
     */
    void clear();

    /** 
     * Checks if array is empty.
     *
     * @return true if array is empty, false otherwise.
     */
    bool empty() const noexcept;

    /** friend declaration of operator << overloading. */
    friend std::ostream& (::operator <<)(std::ostream& os, const imagestego::BitArray& arr);

    /**
     * Iterator to beginning. 
     *
     * @return Iterator pointing to the first element of array.
     */
    iterator begin();

    /**
     * Iterator to end. 
     *
     * @return Iterator pointing to past-the-end element in array.
     */
    iterator end();
}; // class BitArray

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArrayImpl& arr);

/**
 * Operator == overloading.
 *
 * @param lhs Left-hand side array.
 * @param rhs Right-hand side array.
 * @return true iff lhs and rhs are equal.
 */
//bool operator ==(const imagestego::BitArray& lhs, const imagestego::BitArray& rhs);

#endif /* __IMAGESTEGO_BITARRAY_HPP_INCLUDED__ */
