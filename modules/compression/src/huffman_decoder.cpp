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

HuffmanDecoder::HuffmanDecoderImpl::HuffmanDecoderImpl() noexcept {}

HuffmanDecoder::HuffmanDecoderImpl::HuffmanDecoderImpl(const BitArray& arr) noexcept : encodedMsg(arr) {}

HuffmanDecoder::HuffmanDecoderImpl::~HuffmanDecoderImpl() noexcept {
    if (root)
        delete root;
}

HuffmanDecoder::HuffmanDecoderImpl::TreeNode::TreeNode(TreeNode* node) noexcept : parent(node) {}

HuffmanDecoder::HuffmanDecoderImpl::TreeNode::~TreeNode() noexcept {
    if (left)
        delete left;
    if (right)
        delete right;
}

std::string HuffmanDecoder::HuffmanDecoderImpl::getDecodedMessage() {
    if (!root) {
        readDfs();
        readAlphabet();
        createCodeTable();
        decode();
    }
    return decodedMsg;
}

void HuffmanDecoder::HuffmanDecoderImpl::readDfs() {
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

// TODO: optimize this
unsigned char takeChar(const BitArray& arr, const imagestego::size_t& pos) {
    unsigned char tmp = 0;
    for (unsigned short i = 0; i != 8; ++i) {
        tmp |= arr[pos + i] << (7 - i);
    }
    return tmp;
}

void HuffmanDecoder::HuffmanDecoderImpl::readAlphabet() {
    for (imagestego::size_t i = 0; i != codes.size(); ++i) {
        alphabet += takeChar(encodedMsg, it);
        it += 8;
    }
}

void HuffmanDecoder::HuffmanDecoderImpl::createCodeTable() {
    for (imagestego::size_t i = 0; i != codes.size(); ++i)
        codeTable.emplace(codes[i], alphabet[i]);
}

void HuffmanDecoder::HuffmanDecoderImpl::decode() {
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

HuffmanDecoder::HuffmanDecoder() noexcept : decoder(new HuffmanDecoderImpl()) {}

HuffmanDecoder::HuffmanDecoder(const BitArray& arr) noexcept : decoder(new HuffmanDecoderImpl(arr)) {}

HuffmanDecoder::~HuffmanDecoder() noexcept {
    delete decoder;
}

std::string HuffmanDecoder::getDecodedMessage() {
    return decoder->getDecodedMessage();
}

} // namespace imagestego
