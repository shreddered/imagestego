#ifndef __IMAGESTEGO_FORMAT_CHECKER_HPP_INCLUDED__
#define __IMAGESTEGO_FORMAT_CHECKER_HPP_INCLUDED__

#include <algorithm>
#include <string>
#include <vector>


namespace imagestego {

class FormatChecker {
private:
    int fmt;
    std::vector<std::string> formats;
public:
    enum {
        Jpeg = 1,
        Png = 1 << 1,
        Bmp = 1 << 2,
        Tiff = 1 << 3,
        Gif = 1 << 4,
    };
    explicit FormatChecker(int _fmt) noexcept;
    void setFormat(int _fmt) noexcept;
    bool check(const std::string& str);
}; // class FormatChecker

} // namespace imagestego

#endif /* __IMAGESTEGO_FORMAT_CHECKER_HPP_INCLUDED__ */
