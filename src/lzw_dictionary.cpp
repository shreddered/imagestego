#include "imagestego/compression/lzw_dictionary.hpp"


namespace imagestego {

Dictionary::Dictionary() noexcept : newCode(257) {
    codeTable.reserve(256);
    for (unsigned int i = 0; i != 256; ++i)
        codeTable[i] = StringElement(i);
}

int Dictionary::search(const StringElement& s) {
    if (s.prefixIndex == -1)
        return s.value;
    int index = codeTable[s.prefixIndex].first;
    if (index == -1) {
        codeTable[s.prefixIndex].first = newCode;
        codeTable[newCode++] = s;
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
                      codeTable[index].left = newCode;
                      codeTable[newCode++] = s;
                      return -1;
                  }
                  index = left;
              }
              else {
                  int right = codeTable[index].right;
                  if (right == -1) { // right insertion case
                      codeTable[index].right = newCode;
                      codeTable[newCode++] = s;
                      return -1;
                  }
                  index = right;
              }
        } // while(1)
    }
}

} // namespace imagestego
