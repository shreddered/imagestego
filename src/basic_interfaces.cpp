#include <algorithms/basic_interfaces.hpp>


StringRef::StringRef(std::string& _str) noexcept : str(_str) {}

StringSource::StringSource(const std::string& _str) noexcept : str(_str) {}

Pipe::Pipe(StringSource* _source, AbstractStegoInserter* _inserter) : inserter(_inserter), source(_source) {
    inserter->setMessage(source->str); 
    inserter->createStegoContainer();
}

Pipe::Pipe(AbstractStegoInserter* _extracter, StringRef* _ref) : extracter(_extracter), ref(_ref) {
    ref->str = extracter->extractMessage();
}

Pipe::~Pipe() noexcept {
    if (inserter)
        delete inserter;
    if (extracter)
        delete extracter;
    if (ref)
        delete ref;
    if (source)
        delete source;
}
