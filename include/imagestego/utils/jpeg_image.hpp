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

struct Point {
    short& x;
    short& y;
    short& z;
    explicit Point(short& x, short& y, short& z) noexcept;
    short& operator[](int index);
}; // struct

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
    void writeTo(const std::string& dst);
private:
    FILE* in = nullptr;
    jvirt_barray_ptr* coeffs = nullptr;
    jpeg_decompress_struct dinfo;
    jpeg_error_mgr err;
}; // class JpegImage

}

#endif /* __IMAGESTEGO_JPEG_IMAGE_HPP_INCLUDED__ */
