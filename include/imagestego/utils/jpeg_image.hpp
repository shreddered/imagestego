#ifndef __IMAGESTEGO_JPEG_IMAGE_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_IMAGE_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <cstdio>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
// libjpeg
#include <jpeglib.h>


namespace imagestego {

struct Point_ {
    const short& x;
    const short& y;
    const short& z;
    explicit Point_(const short& _x, const short& _y, const short& _z) noexcept;
    const short& operator[](int index) const noexcept;
}; // struct Point_

struct Point {
    short& x;
    short& y;
    short& z;
    explicit Point(short& x, short& y, short& z) noexcept;
    short& operator[](int index) noexcept;
    operator Point_() const {
        return Point_(x, y, z);
    }
}; // struct Point

class JpegImage {
public:
    int rows, cols;

    explicit JpegImage() noexcept;
    explicit JpegImage(const std::string& src);
    virtual ~JpegImage() noexcept;
    void open(const std::string& src);
    void close() noexcept;
    inline bool isEmpty() const noexcept {
        return coeffs == nullptr;
    }
    Point at(const int& y, const int& x);
    Point_ at(const int& y, const int& x) const;
    void writeTo(const std::string& dst);
private:
    FILE* in = nullptr;
    jvirt_barray_ptr* coeffs = nullptr;
    mutable jpeg_decompress_struct dinfo;
    jpeg_error_mgr err;
}; // class JpegImage

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_IMAGE_HPP_INCLUDED__ */
