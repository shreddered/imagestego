#ifndef __BASIC_INTERFACES_HPP_INCLUDED__
#define __BASIC_INTERFACES_HPP_INCLUDED__

#include <string>

#include <utils/bitarray.hpp>

#include <highgui.h>


enum class Mode {
    insertion,
    extraction
};

class AbstractStegoInserter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setImage(const cv::Mat& image) = 0;
    virtual void setOutputName(const std::string& filename) = 0;
    virtual void createStegoContainer() = 0;
    virtual void setMessage(const std::string& msg) = 0;
}; // class AbstractStegoInserter

class AbstractStegoExtracter {
public:
    virtual void setStegoContainer(const std::string& imageName) = 0;
    virtual std::string extractMessage() = 0;
}; // class AbstractStegoExtracter

class Pipe;

class StringRef {
    friend class Pipe;
private:
    std::string& str;
public:
    explicit StringRef(std::string& _str) noexcept;
    virtual ~StringRef() noexcept = default;
}; // class StringRef

class StringSource {
    friend class Pipe;
private:
    const std::string& str;
public:
    explicit StringSource(const std::string& _str) noexcept;
    virtual ~StringSource() noexcept = default;
}; // class StringSource

class Pipe {
public:
    explicit Pipe(StringSource* _source, AbstractStegoInserter* _inserter);
    explicit Pipe(AbstractStegoExtracter* _extracter, StringRef* _ref);
    virtual ~Pipe() noexcept;
private:
    AbstractStegoInserter* inserter = nullptr;
    AbstractStegoExtracter* extracter = nullptr;
    StringSource source = nullptr;
    StringRef* ref = nullptr;
};

#endif /* __BASIC_INTERFACES_HPP_INCLUDED__ */
