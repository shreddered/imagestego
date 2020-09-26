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

#ifndef __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__
#define __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <unordered_map>
#include <string>
#include <vector>


namespace imagestego {

class IMAGESTEGO_EXPORTS HuffmanDecoder : public AbstractDecoder {
public:
    explicit HuffmanDecoder() noexcept;
    explicit HuffmanDecoder(const BitArray& arr) noexcept;
    virtual ~HuffmanDecoder() noexcept;
    void setMessage(const BitArray& arr) override {
        decoder->setMessage(arr);
    }
    std::string getDecodedMessage() override;
private:
    class HuffmanDecoderImpl {
    public:
        explicit HuffmanDecoderImpl() noexcept;
        explicit HuffmanDecoderImpl(const BitArray& arr) noexcept;
        virtual ~HuffmanDecoderImpl() noexcept;
        void setMessage(const BitArray& arr) {
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
        imagestego::size_t it;
        BitArray encodedMsg;
        std::string alphabet;
        std::string decodedMsg;
        std::unordered_map<std::string, char> codeTable;
    }; // class HuffmanDecoderImpl
    HuffmanDecoderImpl* decoder;
}; // class HuffmanDecoder

} // namespace imagestego

#endif /* __IMAGESTEGO_HUFFMAN_DECODER_HPP_INCLUDED__ */
