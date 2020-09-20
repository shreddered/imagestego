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

#include "imagestego/core/bitarray.hpp"
#ifdef IMAGESTEGO_LITTLE_ENDIAN
#   include "imagestego/core/intrinsic.hpp"
#   include <algorithm>
#endif
#include <cstring>
#include <ostream>


namespace imagestego {

// BitReference
BitArrayImpl::BitReference::BitReference(typename BitArrayImpl::BlockType& block, size_t pos) noexcept 
    : _block(block), _mask(1 << (bitsPerBlock - pos - 1)) {}

BitArrayImpl::BitReference::operator bool() const {
    return (_block & _mask) != 0;
}

BitArrayImpl::BitReference& BitArrayImpl::BitReference::operator =(bool val) noexcept {
    if (val)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

BitArrayImpl::BitReference& BitArrayImpl::BitReference::operator =(const BitReference& other) noexcept {
    if (other)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

bool BitArrayImpl::BitReference::operator==(const BitArrayImpl::BitReference& other) noexcept {
    return &other._block == &_block && other._mask == _mask;
}

// BitIterator
BitArrayImpl::BitIterator::BitIterator(typename BitArrayImpl::reference ref) noexcept : _ref(ref) {}

BitArrayImpl::BitReference BitArrayImpl::BitIterator::operator *() {
    return _ref;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator ++() {
    if (_ref._mask == 1) {
        _ref._block = *(&_ref._block + 1);
        _ref._mask = 1 << (bitsPerBlock - 1);
    }
    else
        _ref._mask >>= 1;
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator ++(int) {
    BitIterator it = *this;
    operator ++();
    return it;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator --() {
    if (_ref._mask == 1 << (bitsPerBlock - 1)) {
        _ref._block = *(&_ref._block - 1);
        _ref._mask = 1;
    }
    else
        _ref._mask <<= 1;
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator --(int) {
    BitIterator it = *this;
    operator ++();
    return it;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator +=(size_t i) {
    return *this;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator -=(size_t i) {
    _ref._block = *(&_ref._block - i);
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator +(size_t i) {
    BitIterator it = *this;
    return it += i;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator -(size_t i) {
    BitIterator it = *this;
    return it -= i;
}

bool BitArrayImpl::BitIterator::operator==(const BitIterator& other) noexcept {
    return _ref == other._ref;
}

bool BitArrayImpl::BitIterator::operator!=(const BitIterator& other) noexcept {
    return !(*this == other);
}

// BitArrayImpl
BitArrayImpl::BitArrayImpl() noexcept : _sz(0) {}

BitArrayImpl::BitArrayImpl(size_t sz) : _blocks(numberOfBlocks(sz)), _sz(sz) {}

BitArrayImpl::BitArrayImpl(const std::string& str) : _blocks(numberOfBlocks(str.size())), _sz(str.size()) {
    for (size_t i = 0; i != _sz; ++i)
        operator [](i) = str[i] - '0' != 0;
}

BitArrayImpl::BitReference BitArrayImpl::operator[](size_t i) {
    return BitReference(_blocks[blockIndex(i)], bitIndex(i));
}

bool BitArrayImpl::operator[](size_t i) const {
    return (_blocks[blockIndex(i)] & (1 << (bitsPerBlock - bitIndex(i) - 1))) != 0;
}

size_t BitArrayImpl::size() const noexcept {
    return _sz;
}

BitArrayImpl BitArrayImpl::fromByteString(std::string str) {
    BitArrayImpl arr(str.size() * CHAR_BIT);
    str.append(sizeof(BlockType) - str.size() % sizeof(BlockType), '\0');
    memcpy(&arr._blocks[0], str.data(), str.size());
#ifdef IMAGESTEGO_LITTLE_ENDIAN
    std::for_each(arr._blocks.begin(), arr._blocks.end(), [](uint32_t& value) {
        value = bswap(value);
    });
#endif
    return arr;
}

void BitArrayImpl::pushBack(bool val) {
    if (_sz / CHAR_BIT == _blocks.size())
        _blocks.push_back(0);
    BitReference(_blocks[blockIndex(_sz)], bitIndex(_sz)) = val;
    ++_sz;
}

void BitArrayImpl::put(int num, size_t n) {
    for(; n != 0; --n) {
        const size_t offset = n - 1;
        pushBack((num & (1 << offset)) != 0);
    }
}

void BitArrayImpl::put(size_t num, size_t n) {
    for(; n != 0; --n) {
        const size_t offset = n - 1;
        pushBack((num & (1 << offset)) != 0);
    }
}

void BitArrayImpl::clear() {
    _blocks.clear();
    _sz = 0;
}

bool BitArrayImpl::empty() const noexcept {
    return _sz == 0;
}

typename BitArrayImpl::iterator BitArrayImpl::begin() {
    return BitIterator(operator[](0));
}

typename BitArrayImpl::iterator BitArrayImpl::end() {
    return (_sz / CHAR_BIT == _blocks.size()) 
        ? BitIterator(BitReference(*(&_blocks.back() + 1), 0)) : BitIterator(operator [](_sz));
}

std::string BitArrayImpl::toByteString() const {
    std::string str(_sz / CHAR_BIT, '\0');
#if IMAGESTEGO_LITTLE_ENDIAN
    auto tmp = _blocks;
    std::for_each(tmp.begin(), tmp.end(), [](uint32_t& value) {
        value = bswap(value);
    });
    memcpy(&str[0], &tmp[0], tmp.size() * sizeof(BlockType));
#else
    memcpy(&str[0], &_blocks[0], _blocks.size() * sizeof(BlockType));
#endif
    return str;
}

std::string BitArrayImpl::toString() const {
    std::string str;
    str.reserve(_sz);
    for (size_t i = 0; i != _sz; ++i)
        str.push_back((operator [](i)) ? '1' : '0');
    return str;
}

// BitArray
BitArray::BitArray() : _arr(new BitArrayImpl()) {} 

BitArray::BitArray(const std::string& str) : _arr(new BitArrayImpl(str)) {}

BitArray::BitArray(size_t sz) : _arr(new BitArrayImpl(sz)) {}

BitArray::~BitArray() noexcept {
    if (_arr)
        delete _arr;
}

BitArray::BitArray(const BitArray& other) : _arr(new BitArrayImpl(*other._arr)) {}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this != & other) {
        delete _arr;
        _arr = new BitArrayImpl(*other._arr);
    }
    return *this;
}

BitArray::BitArray(BitArray&& other) noexcept : _arr(other._arr) {
    other._arr = nullptr;
}

BitArray& BitArray::operator=(BitArray&& other) noexcept {
    _arr = other._arr;
    other._arr = nullptr;
    return *this;
}

BitArray BitArray::fromByteString(std::string str) {
    BitArray arr;
    arr._arr = new BitArrayImpl(BitArrayImpl::fromByteString(std::move(str)));
    return arr;
}

typename BitArrayImpl::reference BitArray::operator[](size_t i) {
    return (*_arr)[i];
}

size_t BitArray::size() const noexcept {
    return _arr->size();
}

bool BitArray::operator[](size_t i) const {
    return (*_arr)[i];
}

void BitArray::pushBack(bool val) {
    _arr->pushBack(val);
}

void BitArray::clear() {
    _arr->clear();
}

bool BitArray::empty() const noexcept {
    return _arr->empty();
}

typename BitArray::iterator BitArray::begin() {
    return _arr->begin();
}

typename BitArray::iterator BitArray::end() {
    return _arr->end();
}

std::string BitArray::toByteString() const {
    return _arr->toByteString();
}

std::string BitArray::toString() const {
    return _arr->toString();
}

void BitArray::put(int num, size_t n) {
    _arr->put(num, n);
}

void BitArray::put(size_t num, size_t n) {
    _arr->put(num, n);
}

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArrayImpl& arr) {
    for (size_t i = 0; i != arr.size(); ++i) {
        os << arr[i];
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr) {
    return os << (*arr._arr);
}
