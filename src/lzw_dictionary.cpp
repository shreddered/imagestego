#include "imagestego/compression/lzw_dictionary.hpp"


namespace imagestego {

Dictionary::Dictionary() noexcept : codeTable(256) {
    for (unsigned int i = 0; i != 256; ++i) {
        codeTable[i] = StringElement(i);
    }
}

Dictionary::~Dictionary() = default;

void Dictionary::clear() noexcept {
    codeTable = std::vector<StringElement>(256);
    for (unsigned int i = 0; i != 256; ++i) {
        codeTable[i] = StringElement(i);
    }
    newCode = 256;
}

std::string Dictionary::at(int index) {
    std::string tmp;
    while (index != -1) {
        tmp = char(codeTable[index].value) + tmp;
        index = codeTable[index].prefixIndex;
    }
    return tmp;
}

int Dictionary::search(const StringElement& s) {
    if (s.prefixIndex == -1)
        return s.value;
    int index = codeTable[s.prefixIndex].first;
    if (index == -1) {
        codeTable[s.prefixIndex].first = newCode++;
        codeTable.push_back(s);
        return -1;
    }
    else { // perform search
        while(1) {
              uint8_t val = codeTable[index].value;
              if (s.value == val)
                  return index; 
              if (s.value < val) {
                  int left = codeTable[index].left;
                  if (left == -1) { // left insertion case
                      codeTable[index].left = newCode++;
                      codeTable.push_back(s);
                      return -1;
                  }
                  index = left;
              }
              else {
                  int right = codeTable[index].right;
                  if (right == -1) { // right insertion case
                      codeTable[index].right = newCode++;
                      codeTable.push_back(s);
                      return -1;
                  }
                  index = right;
              }
        } // while(1)
    }
}

void Dictionary::add(const uint8_t& value, const int& prefixIndex) {
    codeTable.emplace_back(value, prefixIndex);
    ++newCode;
}

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::StringElement& elem) {
    os << elem.prefixIndex << ' ' << elem.value << ' ' << elem.first << ' ' << elem.left << ' ' << elem.right;
}
