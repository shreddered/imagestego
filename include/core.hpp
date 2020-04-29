#ifndef __IMAGESTEGO_CORE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_HPP_INCLUDED__

#include <string>

#include <utils/bitarray.hpp>

#include <opencv2/imgcodecs.hpp>


enum class Mode {
    insertion,
    extraction
};

class AbstractStegoInserter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setOutputName(const std::string& filename) = 0;
    virtual void createStegoContainer() const = 0;
    virtual void setMessage(const std::string& msg) = 0;
    virtual ~AbstractStegoInserter() = default;
}; // class AbstractStegoInserter

class AbstractStegoExtracter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual std::string extractMessage() = 0;
    virtual ~AbstractStegoExtracter() = default;
}; // class AbstractStegoExtracter

class AbstractOutputDevice {
public:
    virtual void put(const char* data, std::size_t len) = 0;
    virtual void end() noexcept = 0;
}; // class AbstractOutputDevice

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
    explicit StringSource(const std::string& _str, AbstractOutputDevice* device); 
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
    StringSource* source = nullptr;
    StringRef* ref = nullptr;
};

class HexEncoder {
public:
    explicit HexEncoder(StringRef* src) noexcept;
    std::string getHexString() noexcept;
private:

}; // class HexEncoder

#endif /* __IMAGESTEGO_CORE_HPP_INCLUDED__ */
