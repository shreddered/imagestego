/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __IMAGESTEGO_CORE_BITARRAY_IMPL_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_BITARRAY_IMPL_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
// c++ headers
#include <ostream>
#include <string>
#include <vector>

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

namespace imagestego {

namespace impl {

/**
 * Implementation of class BitArray for working with bit sequences.
 */
class BitArray final {
    class BitReference;
    class BitIterator;

public:
    typedef BitReference reference;
    typedef BitIterator iterator;
    typedef const BitIterator const_iterator;

    bool operator==(const BitArray& other);

    /**
     * Constructor of BitArray.
     *
     * Constructs empty array.
     */
    explicit BitArray() noexcept;

    /**
     * Constructor of BitArray.
     *
     * Constructs array from binary string.
     *
     * @param str String containing '1' and '0'.
     */
    BitArray(const std::string& str);

    /**
     * Constructor of BitArray.
     *
     * Constructs array with given size.
     *
     * @param sz Size of array.
     */
    explicit BitArray(std::size_t sz);

    /**
     * Converter from byte sequence to bit array.
     *
     * @param str Byte string to be converted to bit array.
     * @return Converted array.
     */
    static BitArray fromByteString(std::string str);

    /**
     * Creates array from unsigned 32-bit integer type.
     *
     * @param num Number to be converted to bit array.
     * @return Created array.
     */
    static BitArray fromInt(uint32_t num);

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
     * Converts bit array to int.
     *
     * @return Unsigned 32-bit integer.
     */
    std::size_t toInt() const;

    /**
     * Size of array.
     *
     * @return Size of array
     */
    std::size_t size() const noexcept;

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Instance of proxy class representing element.
     */
    BitReference operator[](std::size_t i);

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Read-only element of array.
     */
    bool operator[](std::size_t i) const;

    /**
     * Adds element to the end.
     *
     * @param val Value to be added
     */
    void pushBack(bool val);

    /**
     * Puts n bits from number to array.
     *
     * @param num Number from which bits are taken.
     * @param n Nubmer of bits to take from num.
     */
    void put(std::size_t num, std::size_t n);

    /**
     * Pushes unsigned 32-bit integer in the beginning of the array.
     *
     * @param num number to be pushed.
     */
    void pushFront(std::size_t num);

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
        friend class BitArray;
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
        explicit BitReference(BlockType& block, std::size_t pos) noexcept;

    public:
        BitReference(const BitReference&) = default;

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
        BitReference& operator=(bool val) noexcept;

        /**
         * Operator = overloading.
         *
         * @param val Reference to be assigned.
         * @return This instance's reference.
         */
        BitReference& operator=(const BitReference& other) noexcept;

        /**
         * Comparator for two references.
         *
         * @return true if references are equal, false otherwise.
         */
        bool operator==(const BitReference& other) noexcept;
    }; // class BitReference

    /**
     * Bit array iterator class.
     */
    class IMAGESTEGO_EXPORTS BitIterator final {
    private:
        friend class BitArray;

        /** Iterator's owner. */
        BitArray* _owner;

        /** Current position. */
        std::size_t _pos;

        /**
         * Constructs iterator.
         *
         * @param owner Iterator's owner.
         * @param pos Position of element iterator points to.
         */
        explicit BitIterator(BitArray* owner, std::size_t pos) noexcept;

    public:
        /**
         * Unary * overloading.
         *
         * @return Reference iterator points to.
         */
        BitReference operator*();

        /**
         * Prefix increment overloading.
         *
         * @return This instance's reference.
         */
        BitIterator& operator++();

        /**
         * Postfix increment overloading.
         *
         * @return Iterator befor it was incremented.
         */
        BitIterator operator++(int);

        /**
         * Prefix decrement overloading.
         *
         * @return This instance's reference.
         */
        BitIterator& operator--();

        /**
         * Postfix decrement overloading.
         *
         * @return Iterator befor it was decremented.
         */
        BitIterator operator--(int);

        // BitIterator& operator+=(std::size_t i);

        // BitIterator& operator-=(std::size_t i);

        // BitIterator operator+(std::size_t i);

        // BitIterator operator-(std::size_t i);

        /**
         * Iterator comparison.
         */
        bool operator==(const BitIterator& other) noexcept;
        bool operator!=(const BitIterator& other) noexcept;
    }; // class BitIterator

    /** Array of integer blocks. */
    std::vector<BlockType> _blocks;

    /** Size of array. */
    std::size_t _sz;

    /** Number of bits per block. */
    static IMAGESTEGO_CONSTEXPR std::size_t bitsPerBlock = CHAR_BIT * sizeof(BlockType);

    /**
     * Number of blocks needed to store sz bits.
     *
     * @param sz Number of bits.
     * @return Number of blocks needed to store sz bits.
     */
    inline static IMAGESTEGO_CONSTEXPR std::size_t numberOfBlocks(std::size_t sz) {
        return sz / bitsPerBlock + ((sz % bitsPerBlock != 0) ? 1 : 0);
    }

    /**
     * Block index of i-th element.
     *
     * @param i Index of an element.
     * @return Index of block containing i-th element.
     */
    inline static IMAGESTEGO_CONSTEXPR std::size_t blockIndex(std::size_t i) {
        return i / bitsPerBlock;
    }

    /**
     * Bit index of i-th element.
     *
     * @param i index of an element.
     * @return Index of bit in the block with i-th element.
     */
    inline static IMAGESTEGO_CONSTEXPR std::size_t bitIndex(std::size_t i) {
        return i % bitsPerBlock;
    }
}; // class BitArray

} // namespace impl

} // namespace imagestego

std::ostream& operator<<(std::ostream& os, const imagestego::impl::BitArray& arr);

#endif /* __IMAGESTEGO_CORE_BITARRAY_IMPL_HPP_INCLUDED__ */
