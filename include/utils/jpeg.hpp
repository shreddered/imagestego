#ifndef __IMAGESTEGO_JPEG_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_HPP_INCLUDED__

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <utility>

#include <jpeglib.h>

/*
 * TODO: reduce amount of code
 */ 
class JpegProcessor {
public:
    explicit JpegProcessor(const std::string& src);
    virtual ~JpegProcessor() noexcept;
    virtual void process() const = 0;
    void writeTo(const std::string& dst) const;
protected:
    JCOEFPTR getBlock(const int& channel, const int& y, const int& x) const;
    std::pair<int, int> getChannelSize(const int& channel) const;
private:
    FILE* input;
    mutable jpeg_decompress_struct decompressInfo;
    jpeg_error_mgr err;
    jvirt_barray_ptr* dctCoeffs;
}; // class JpegImage

#endif /* __IMAGESTEGO_JPEG_HPP_INCLUDED__ */
