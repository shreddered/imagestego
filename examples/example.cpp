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
    //std::cout << "Hello, world!" << a <<std::endl;
    /*Lsb lsb;
    lsb.setMessage("Hello!");
    lsb.setImage("nice_man.jpg");
    lsb.setOutputName("nice_stego.png");
    lsb.createStegoContainer();*/
    /*Lsb lsb;
    lsb.setImage("nice_stego.png");
    std::cout << lsb.extractMessage() << std::endl;
    */
    int mode;
    getopts(argc, argv, "mode", &mode);
    if (mode == 1) { // create stego case
        std::string image, output, message;
        getopts(argc, argv, "image", &image, "output", &output, "message", &message);
        Pipe p(new StringSource(message), new Lsb(image, output));
    }
    else if (mode == 2) {
        std::string container, message;
        getopts(argc, argv, "image", &container);
        Pipe p(new Lsb(container), new StringRef(message));
        std::cout << message << std::endl;
    }
    return 0;
}
