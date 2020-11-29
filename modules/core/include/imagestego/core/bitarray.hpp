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

#ifndef __IMAGESTEGO_CORE_BITARRAY_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_BITARRAY_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/bitarrayimpl.hpp"
// c++ headers
#include <ostream>
#include <string>

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
 * Class representing bit array.
 */
class IMAGESTEGO_EXPORTS BitArray {
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
    explicit BitArray(std::size_t sz);

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
    static BitArray fromByteString(const std::string& str);

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
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Instance of proxy class representing element.
     */
    typename BitArrayImpl::reference operator[](std::size_t i);

    /**
     * Operator [] overloading.
     *
     * @param i Index of element in array.
     * @return Read-only element of array.
     */
    bool operator [](std::size_t i) const;

    /**
     * Size of array.
     *
     * @return Size of array
     */
    std::size_t size() const noexcept;

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

    bool operator ==(const BitArray& other);

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
     * Constructs array from implementation pointer.
     *
     * @param arr Pointer to implementation.
     */
    explicit BitArray(BitArrayImpl* arr) noexcept;

    /** Pointer to implementation. */
    BitArrayImpl* _arr;
}; // class BitArray

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_BITARRAY_HPP_INCLUDED__ */
