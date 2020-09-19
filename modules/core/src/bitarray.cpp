#include "imagestego/core/bitarray.hpp"
#ifdef IMAGESTEGO_BIG_ENDIAN
#   include "imagestego/core/intrinsic.hpp"
#   include <algorithm>
#endif
#include <cstring>
#include <ostream>


namespace imagestego {

// BitReference
BitArrayImpl::BitReference::BitReference(typename BitArrayImpl::BlockType& block, std::size_t pos) noexcept 
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

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator +=(std::size_t i) {
    return *this;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator -=(std::size_t i) {
    _ref._block = *(&_ref._block - i);
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator +(std::size_t i) {
    BitIterator it = *this;
    return it += i;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator -(std::size_t i) {
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

BitArrayImpl::BitArrayImpl(std::size_t sz) : _blocks(numberOfBlocks(sz)), _sz(sz) {}

BitArrayImpl::BitArrayImpl(const std::string& str) : _blocks(numberOfBlocks(str.size())), _sz(str.size()) {
    for (std::size_t i = 0; i != _sz; ++i)
        operator [](i) = str[i] - '0' != 0;
}

BitArrayImpl::BitReference BitArrayImpl::operator[](std::size_t i) {
    return BitReference(_blocks[blockIndex(i)], bitIndex(i));
}

bool BitArrayImpl::operator[](std::size_t i) const {
    return (_blocks[blockIndex(i)] & (1 << (bitsPerBlock - bitIndex(i) - 1))) != 0;
}

std::size_t BitArrayImpl::size() const noexcept {
    return _sz;
}

BitArrayImpl BitArrayImpl::fromByteString(std::string str) {
    BitArrayImpl arr(str.size() * CHAR_BIT);
    str.append((sizeof(BlockType) - str.size()) % sizeof(BlockType), '\0');
    memcpy(&arr._blocks[0], str.data(), str.size());
#ifdef IMAGESTEGO_BIG_ENDIAN
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

typename BitArrayImpl::iterator BitArrayImpl::begin() {
    return BitIterator(operator[](0));
}

typename BitArrayImpl::iterator BitArrayImpl::end() {
    return (_sz / CHAR_BIT == _blocks.size()) 
        ? BitIterator(BitReference(*(&_blocks.back() + 1), 0)) : BitIterator(operator [](_sz));
}

std::string BitArrayImpl::toByteString() {
    std::string str(_sz / CHAR_BIT, '\0');
#if IMAGESTEGO_BIG_ENDIAN
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

// BitArray
BitArray::BitArray() : _arr(new BitArrayImpl()) {} 

BitArray::BitArray(const std::string& str) : _arr(new BitArrayImpl(str)) {}

BitArray::BitArray(std::size_t sz) : _arr(new BitArrayImpl(sz)) {}

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

typename BitArrayImpl::reference BitArray::operator[](std::size_t i) {
    return (*_arr)[i];
}

std::size_t BitArray::size() const noexcept {
    return _arr->size();
}

bool BitArray::operator[](std::size_t i) const {
    return (*_arr)[i];
}

void BitArray::pushBack(bool val) {
    _arr->pushBack(val);
}

typename BitArray::iterator BitArray::begin() {
    return _arr->begin();
}

typename BitArray::iterator BitArray::end() {
    return _arr->end();
}

std::string BitArray::toByteString() {
    return _arr->toByteString();
}

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArrayImpl& arr) {
    for (std::size_t i = 0; i != arr.size(); ++i) {
        os << arr[i];
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr) {
    return os << (*arr._arr);
}
