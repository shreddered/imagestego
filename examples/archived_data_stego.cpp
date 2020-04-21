#include <iostream>

#include <algorithms/lsb_stego.hpp>
#include <encoder.hpp>
#include <decoder.hpp>

#include <algorithm>
#include <bitset>
#include <iostream>


std::string startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.size() >= str.size())
        return "";
    int i;
    for (i = 0; i < prefix.size(); ++i)
        if (str[i] != prefix[i])
            return "";
    return std::string(str.begin() + i, str.end());
    
}

template<typename T>
T fromString(const std::string& str);

template<>
int fromString(const std::string& str) {
    return stoi(str);
}

template<>
std::string fromString(const std::string& str) {
    return str;
}

void getopts(const int& argc, char** agrv) {}

template<typename T, typename... Args>
void getopts(const int& argc, char** argv, const std::string& arg, T* value, Args... args) {
    std::for_each(argv, argv + argc, [&arg, &value](const char* str) {
        auto s = startsWith(str, "--" + arg + '=');
        if (!s.empty())
            *value = fromString<T>(s);
    });
    getopts(argc, argv, args...);
} 

int main(int argc, char** argv) {
    auto b = BitArray<>::fromInt(4);
    std::cout << b << std::endl;
    //test
    if (argv[1] == std::string("insert")) {
        std::string msg, key, output, image;
        getopts(argc, argv,
                "msg", &msg,
                "key", &key,
                "output", &output,
                "image", &image);
        Lsb lsb(image, output, LsbOptions::randomBits);
        lsb.setSecretKey(key);
        HuffmanEncoder enc(msg);
        lsb.setMessage(enc.getEncodedMessage());
        lsb.createStegoContainer();
    }
    else {
        std::string image, key;
        getopts(argc, argv,
                "image", &image,
                "key", &key);
        Lsb lsb(LsbOptions::randomBits);
        lsb.setSecretKey(key);
        lsb.setStegoContainer(image);
        auto msg = BitArray<unsigned char> (lsb.extractMessage());
        std::cout << msg << std::endl;
        std::cout << HuffmanDecoder(msg).getDecodedMessage() << std::endl;

    }
    return 0;
}
