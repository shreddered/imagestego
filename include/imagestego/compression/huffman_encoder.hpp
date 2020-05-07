#ifndef __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__
#define __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__

// imagestego 
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++
#include <algorithm>
#include <cstddef>
#include <map>
#include <string>


namespace imagestego {

class IMAGESTEGO_EXPORTS HuffmanEncoder {
public:
    explicit HuffmanEncoder() noexcept;
    explicit HuffmanEncoder(const std::string& str) noexcept;
    explicit HuffmanEncoder(std::string&& str) noexcept;
    HuffmanEncoder(const HuffmanEncoder&) = delete;
    HuffmanEncoder& operator =(const HuffmanEncoder&) = delete;
    void setMessage(const std::string& str) noexcept;
    void setMessage(std::string&& str) noexcept;
    BitArray<unsigned char> getEncodedMessage() const;
    std::string getHuffmanTree() const;
    std::string getAlphabet() const noexcept {
        return alphabet;
    }
    virtual ~HuffmanEncoder() noexcept;
private:
    struct TreeNode {
        std::string data;
        bool isVisited = false;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        TreeNode(const std::string& data) noexcept : data(data) {}
    };
    void __buildCode() const;
    void encode() const;
    void dfs(TreeNode* node) const;
    void destroyNode(TreeNode* node) noexcept;
    std::string msg;
    mutable TreeNode* root = nullptr;
    mutable std::string encodedMsg = ""; // mutable bc of caching
    mutable std::map<char, std::string> codeTable; 
    mutable std::string route = "";
    mutable std::string alphabet;
}; // class HuffmanEncoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_ENCODER_HPP_INCLUDED__ */
