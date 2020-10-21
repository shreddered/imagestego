# CMake module for detecting compiler and target CPU
# Defines:
#   - IMAGESTEGO_GCC, IMAGESTEGO_ICC, IMAGESTEGO_CLANG
#   - X86/X86_64
#   - AARCH64/ARM
#   - PPC64LE/PPC64


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(IMAGESTEGO_GCC TRUE)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(IMAGESTEGO_CLANG TRUE)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    set(IMAGESTEGO_ICC)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(IMAGESTEGO_MSVC)
else()
    message(STATUS "Unsupported compiler")
endif()

message(STATUS "Target CPU: ${CMAKE_SYSTEM_PROCESSOR}")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64.*|amd64.*|AMD64.*")
    set(X86_64 TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86.*|i686.*|i386.*")
    set(X86 TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64.*|AARCH64.*|arm64.*|ARM64.*")
    set(AARCH64 TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm.*|ARM.*")
    set(ARM TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "(powerpc|ppc)64le")
    set(PPC64LE TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "(powerpc|ppc)64")
    set(PPC64 TRUE)
else()
    message(WARNING "imagestego: can't detect target CPU")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND X86_64)
    set(X86_64 FALSE)
    set(X86 TRUE)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND AARCH64)
    set(AARCH64 FALSE)
    set(ARM TRUE)
endif()

# TODO: add c++11 test
