#ifndef __ENCODER_HPP_INCLUDED__
#define __ENCODER_HPP_INCLUDED__

#include <algorithm>
#include <cstddef>
#include <map>
#include <string>

#include <utils/bitarray.hpp>


class HuffmanEncoder {
public:
    explicit HuffmanEncoder() noexcept;
    explicit HuffmanEncoder(const std::string& str) noexcept;
    HuffmanEncoder(const HuffmanEncoder&) = delete;
    HuffmanEncoder& operator =(const HuffmanEncoder&) = delete;
    void setMessage(const std::string& str) noexcept;
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
    std::string msg = "";
    mutable TreeNode* root = nullptr;
    mutable std::string encodedMsg = ""; // mutable bc of caching
    mutable std::map<char, std::string> codeTable; 
    mutable std::string route = "";
    mutable std::string alphabet;
}; // class HuffmanEncoder

#endif /* __ENCODER_HPP_INCLUDED__ */
