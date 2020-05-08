#ifndef __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__
#define __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__

#include <string>
#include <vector>

#include <ostream>
#include <iostream>


namespace imagestego {

struct StringElement {
    int prefixIndex; // index of prefix string
    uint8_t value; // last byte value stored instead of string
    int first = -1; // index of first string which has *this as prefix
    int left = -1, right = -1; // indexes of string which have same prefix as *this
    explicit constexpr StringElement(const uint8_t val = 0, const int& prefix = -1) noexcept : prefixIndex(prefix), value(val) {}
}; // struct StringElement

class Dictionary {
public:
    explicit Dictionary() noexcept;
    int search(const StringElement& s);
    void clear() noexcept;
    inline unsigned int size() const noexcept {
        return newCode;
    }
    virtual ~Dictionary();
protected:
    std::string at(int code);
    void add(const uint8_t& value, const int& prefixIndex);
private:
    std::vector<StringElement> codeTable;
    unsigned int newCode = 256;
}; // class Dictionary

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::StringElement& elem);

#endif /* __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__ */
