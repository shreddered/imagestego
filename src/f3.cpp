#include "imagestego/algorithms/f3.hpp"


namespace imagestego {

F3Embedder<void>::F3Embedder() noexcept {}

F3Embedder<void>::F3Embedder(const std::string& input, const std::string& _output) : image(input), output(_output) {}

void F3Embedder<void>::setImage(const std::string& imageName) {
    image.open(imageName);
}

void F3Embedder<void>::setOutputName(const std::string& filename) {
    output = filename;
}

void F3Embedder<void>::setMessage(const std::string& _msg) {
    msg = BitArray<>(_msg);
}

void F3Embedder<void>::setSecretKey(const std::string& key) {
    gen.seed(hash(key));
    hasKey = true;
}

Algorithm F3Embedder<void>::getAlgorithm() const noexcept {
    return Algorithm::F3;    
} 

void F3Embedder<void>::createStegoContainer() {
    if (!hasKey) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        auto s = keygen::generate();
        std::cout << "key = " << s << std::endl;
        setSecretKey(s);
#else
        throw Exception(Exception::Codes::NoKeyFound);
#endif 
    }
    randomize(msg, gen);
    msg.put('\0');
    auto lsb = [](const short& value) -> bool {
        return (value & 1) != 0;
    };
    auto decrement = [](short& value) -> void {
        if (value > 0)
            --value;
        else
            ++value;
    };
    std::size_t msgIndex = 0;
    for (int i = 0; i != image.rows; ++i) {
        for (int j = 0; j != image.cols; ++j) {
            auto p = image.at(i, j);
            for (char k = 0; k != 3; ++k) {
                if (p[k]) {
                    if (lsb(p[k]) != msg[msgIndex]) {
                        if (p[k] == 1 || p[k] == -1)
                            p[k] = 0;
                        else {
                            decrement(p[k]);
                            ++msgIndex;
                        }
                    }
                    else
                        ++msgIndex;
                }
                if (msgIndex == msg.size()) {
                    image.writeTo(output);
                    return;
                }
            }
        }
    }
}

F3Extracter<void>::F3Extracter() noexcept {}

F3Extracter<void>::F3Extracter(const std::string& input) : image(input) {}

void F3Extracter<void>::setImage(const std::string& imageName) {
    image.open(imageName);
}

void F3Extracter<void>::setSecretKey(const std::string& key) {
    gen.seed(hash(key));
    hasKey = true;
}

Algorithm F3Extracter<void>::getAlgorithm() const noexcept {
    return Algorithm::F3;
}

std::string F3Extracter<void>::extractMessage() {
    if (!hasKey)
        throw Exception(Exception::Codes::NoKeyFound);
    auto lsb = [](const short& value) -> bool {
        return (value & 1) != 0;
    };
    BitArray<uint8_t> msg;
    for (int i = 0; i != image.rows; ++i) {
        for (int j = 0; j != image.cols; ++j) {
            auto p = image.at(i, j);
            for (char k = 0; k != 3; ++k) {
                if (p[k])
                    msg.pushBack(lsb(p[k]));
                if (msg.size() && msg.size() % 8 == 0 && msg.lastBlock() == 0) {
                    randomize(msg, gen);
                    auto s = msg.toString();
                    s.pop_back();
                    return s;
                }
            }
        }
    }
}

} // namespace imagestego
