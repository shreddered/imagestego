#include "imagestego/utils/jpeg_permutation.hpp"


namespace imagestego {

Permutation::Permutation() noexcept {}

Permutation::Permutation(const int& dim, bool _dir) : mat(dim), dir(_dir) {}

void Permutation::inverse() const {
    std::vector<int> tmp(mat);
    for (int i = 0; i != mat.size(); ++i) {
        tmp[ mat[i] ] = i;       
    }
    mat = tmp;
}

void Permutation::generate(std::mt19937& gen) {
    std::iota(mat.begin(), mat.end(), 0);
    imagestego::shuffle(mat.begin(), mat.end(), gen);
}

JpegPermutation::JpegPermutation(const std::string& src) : JpegProcessor(src) {
    auto size = JpegProcessor::getChannelSize(0);
    p1 = Permutation(size.first << 3, Direction::vertical);
    p2 = Permutation(size.second << 3, Direction::horizontal);
}

std::vector<short> JpegPermutation::getRow(const int& row, const int& channel) const {
    auto rows = JpegProcessor::getChannelSize(channel).first << 3;
    std::vector<short> result(rows);
    for (int i = 0; i < rows; ++i) {
        result[i] = JpegProcessor::at(i, 0, channel);
    }
    return result;
}

void JpegPermutation::setRow(const int& row, const std::vector<short>& arr, const int& channel) const {
    for (int i = 0; i != arr.size(); ++i) {
        JpegProcessor::at(i, 0, channel) = arr[i];
    }
}

std::vector<short> JpegPermutation::getCol(const int& col, const int& channel) const {
    auto cols = JpegProcessor::getChannelSize(channel).second << 3;
    std::vector<short> result(cols);
    for (int i = 0; i < cols; ++i) {
        result[i] = JpegProcessor::at(0, i, channel);
    }
    return result;
}

void JpegPermutation::setCol(const int& col, const std::vector<short>& arr, const int& channel) const {
    for (int i = 0; i != arr.size(); ++i) {
        JpegProcessor::at(0, i, channel) = arr[i]; 
    }
}

void JpegPermutation::generate(const std::string& key) {
    gen.seed(hash(key));
    p1.generate(gen);
    p2.generate(gen);
}

void JpegPermutation::process() const {
    auto size = JpegProcessor::getChannelSize(0);
    auto rows = size.first >> 3,
         cols = size.second >> 3;
    for (int channel = 0; channel != 3; ++channel) {
        for (int i = 0; i != rows; ++i) {
            std::vector<short> row = getRow(i, channel);
            row = row * p1;
            setRow(i, row, channel);
        }
    }
    for (int channel = 0; channel != 3; ++channel) {
        for (int i = 0; i != cols; ++i) {
            std::vector<short> col = getCol(i, channel);
            col = p2 * col;
            setCol(i, col, channel);
        }
    }
    p1.inverse();
    p2.inverse();
}

} // namespace imagestego

std::vector<short> operator *(const imagestego::Permutation& p, const std::vector<short>& col) {
    std::vector<short> result(col);
    for (int i = 0; i != result.size(); ++i) {
        result[i] = col[ p.mat[i] ];
    }
    return result;
}

std::vector<short> operator *(const std::vector<short>& row, const imagestego::Permutation& p) {
    return p * row; // however it's not commutative
}
