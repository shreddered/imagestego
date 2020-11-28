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

#include "imagestego/compression/huffman_decoder.hpp"


namespace imagestego {

// TODO: optimize this
unsigned char takeChar(const BitArray& arr, const std::size_t& pos) {
    unsigned char tmp = 0;
    for (unsigned short i = 0; i != 8; ++i) {
        tmp |= arr[pos + i] << (7 - i);
    }
    return tmp;
}


class HuffmanDecoderImpl {
public:
    explicit HuffmanDecoderImpl() noexcept {}
    explicit HuffmanDecoderImpl(const BitArray& arr) noexcept : encodedMsg(arr) {}
    virtual ~HuffmanDecoderImpl() noexcept {
        if (root)
            delete root;
    }
    void setMessage(const BitArray& arr) {
        encodedMsg = arr;
    }
    std::string getDecodedMessage() {
        if (!root) {
            readDfs();
            readAlphabet();
            createCodeTable();
            decode();
        }
        return decodedMsg;
    }
private:
    struct TreeNode final {
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        TreeNode* parent = nullptr;
        explicit TreeNode(TreeNode* node = nullptr) noexcept : parent(node) {}
        ~TreeNode() noexcept {
            if (left)
                delete left;
            if (right)
                delete right;
        }
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
    void readDfs() {
        root = new TreeNode();
        auto currentNode = root;
        it = 0;
        std::string code;
        do {
            // 1 - go down
            // 0 - up
            if (encodedMsg[it]) {
                currentNode->left = new TreeNode(currentNode);
                currentNode = currentNode->left;
                code += '0';
                if (!encodedMsg[it + 1]) {
                    codes.push_back(code);
                }
            }
            else {
                TreeNode* cameFrom;
                do {
                    cameFrom = currentNode;
                    currentNode = currentNode->parent;
                    code.pop_back();
                } while (currentNode && currentNode->right == cameFrom && currentNode != root);
                if (isLeftChild(cameFrom)) {
                    currentNode->right = new TreeNode(currentNode);
                    currentNode = currentNode->right;
                    code += '1';
                }
                if (!encodedMsg[it + 1] && currentNode != root)
                    codes.push_back(code);
            }
            ++it;
        } while (currentNode != root);
    }
    void readAlphabet() {
        for (std::size_t i = 0; i != codes.size(); ++i) {
            alphabet += takeChar(encodedMsg, it);
            it += 8;
        }
    }
    void buildCode();
    void createCodeTable() {
        for (std::size_t i = 0; i != codes.size(); ++i)
            codeTable.emplace(codes[i], alphabet[i]);
    }
    void decode() {
        auto currNode = root;
        std::string code;
        for ( ; it != encodedMsg.size(); ++it) {
            if (encodedMsg[it]) {
                currNode = currNode->right;
                code.push_back('1');
            }
            else {
                currNode = currNode->left;
                code.push_back('0');
            }
            if (!currNode->left) {
                decodedMsg += codeTable[code];
                code.clear();
                currNode = root;
            }
        }
    }
    std::vector<std::string> codes;
    std::size_t it;
    BitArray encodedMsg;
    std::string alphabet;
    std::string decodedMsg;
    std::unordered_map<std::string, char> codeTable;
}; // class HuffmanDecoderImpl

HuffmanDecoder::HuffmanDecoder() noexcept : decoder(new HuffmanDecoderImpl()) {}

HuffmanDecoder::HuffmanDecoder(const BitArray& arr) noexcept : decoder(new HuffmanDecoderImpl(arr)) {}

HuffmanDecoder::~HuffmanDecoder() noexcept {
    delete decoder;
}

void HuffmanDecoder::setMessage(const BitArray& arr) {
    decoder->setMessage(arr);
}

std::string HuffmanDecoder::getDecodedMessage() {
    return decoder->getDecodedMessage();
}

} // namespace imagestego
