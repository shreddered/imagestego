#include <imagestego.hpp>
#include <utils/bitarray.hpp>
#include <algorithms/lsb_stego.hpp>

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
    if (argv[1] == std::string("insert")) {
        std::string image, output, msg, key;
        getopts(argc, argv,
                "image", &image,
                "output", &output,
                "msg", &msg,
                "key", &key);
        Lsb lsb(image, output, LsbOptions::randomBits);
        lsb.setSecretKey(key);
        lsb.setMessage(msg);
        lsb.createStegoContainer();
    }
    else if (argv[1] == std::string("extract")) {
        std::string container, msg, key;
        getopts(argc, argv, 
                "image", &container,
                "key", &key);
        std::cout << "Here!" << std::endl;
        Lsb lsb(LsbOptions::randomBits);
        lsb.setStegoContainer(container);
        lsb.setSecretKey(key);
        msg = lsb.extractMessage();
        std::cout << msg << std::endl;
    }
    return 0;
}
