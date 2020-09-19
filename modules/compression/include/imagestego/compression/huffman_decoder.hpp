#ifndef __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__
#define __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__

// imagestego
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++
#include <unordered_map>
#include <string>
#include <vector>


namespace imagestego {

class IMAGESTEGO_EXPORTS HuffmanDecoder {
private:
    class HuffmanDecoderImpl {
    public:
        explicit HuffmanDecoderImpl() noexcept;
        explicit HuffmanDecoderImpl(const BitArray<unsigned char>& arr) noexcept; 
        virtual ~HuffmanDecoderImpl() noexcept;
        template<typename Int>
        void setMessage(const BitArray<Int>& arr) {
            encodedMsg = arr;
        }
        std::string getDecodedMessage();
    private:
        struct TreeNode final {
            TreeNode* left = nullptr;
            TreeNode* right = nullptr;
            TreeNode* parent = nullptr;
            explicit TreeNode(TreeNode* node = nullptr) noexcept;
            ~TreeNode() noexcept;
        }; // TreeNode
        TreeNode* root = nullptr;
        static inline bool isLeftChild(TreeNode* node) {
            if (node == nullptr)
                return false;
            if (node->parent == nullptr)
                return false;
            return node->parent->left == node;
        }
        static inline bool isRightChild(TreeNode* node) {
            if (node == nullptr)
                return false;
            if (node->parent == nullptr)
                return false;
            return node->parent->right == node;
        }
        void readDfs();
        void readAlphabet();
        void buildCode();
        void createCodeTable();
        void decode();
        std::vector<std::string> codes;
        std::size_t it;
        BitArray<unsigned char> encodedMsg;
        std::string alphabet;
        std::string decodedMsg;
        std::unordered_map<std::string, char> codeTable;
    }; // class HuffmanDecoderImpl
    HuffmanDecoderImpl* decoder;
public:
    explicit HuffmanDecoder() noexcept;
    explicit HuffmanDecoder(const BitArray<unsigned char>& arr) noexcept; 
    virtual ~HuffmanDecoder() noexcept;
    template<typename Int>
    void setMessage(const BitArray<Int>& arr) {
        decoder->setMessage(arr);
    }
    std::string getDecodedMessage();
}; // class HuffmanDecoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__ */
