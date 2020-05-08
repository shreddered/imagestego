#include "imagestego/utils/format_checker.hpp"


template<class RAIt, typename T = typename RAIt::value_type>
bool binarySearch(RAIt first, RAIt last, const T& data) {
    --last;
    while (first != last) {
        auto m = (last - first + 1) / 2;
        if (*(first + m) > data)
            last = first + m - 1;
        else
            first = first + m;
    }
    return *first == data;
}

namespace imagestego {

FormatChecker::FormatChecker(int _fmt) noexcept : fmt(_fmt) {
    switch(fmt) {
        case Jpeg:
            formats = std::vector<std::string>({ ".jfif",".jpeg", ".jpg" });
            break;
        case Png:
            formats = std::vector<std::string>({ ".png" });
            break;
        case Bmp:
            formats = std::vector<std::string>({ ".bmp", ".dib", ".rle" });
            break;
        case Tiff:
            formats = std::vector<std::string>({ ".tif" });
            break;
        case Gif:
            formats = std::vector<std::string>({ ".gif" });
            break;
    }
}

void FormatChecker::setFormat(int _fmt) noexcept {
    fmt = _fmt;
}

bool FormatChecker::check(const std::string& str) {
    auto n = str.rfind('.');
    auto ext = str.substr(n);
    return binarySearch(formats.begin(), formats.end(), ext); 
}

} // namespace imagestego
