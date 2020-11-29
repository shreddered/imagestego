# CMake module for detecting compiler and target CPU
# Defines:
# - IMAGESTEGO_GCC, IMAGESTEGO_ICC, IMAGESTEGO_CLANG
# - X86/X86_64
# - AARCH64/ARM
# - PPC64LE/PPC64


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(IMAGESTEGO_GCC TRUE)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(IMAGESTEGO_CLANG TRUE)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
  set(IMAGESTEGO_ICC TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(IMAGESTEGO_MSVC TRUE)
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

if (WIN32 AND CMAKE_SYSTEM_NAME MATCHES Windows)
  set(IMAGESTEGO_WIN 1)
endif()

if (MSVC)
  if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(IMAGESTEGO_ARCH "x64")
  elseif("${CMAKE_GENERATOR_PLATFORM}" MATCHES "ARM64")
    set(IMAGESTEGO_ARCH "ARM64")
  elseif("${CMAKE_GENERATOR}" MATCHES "ARM")
    set(IMAGESTEGO_ARCH "ARM")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IMAGESTEGO_ARCH "x64")
  else()
    set(IMAGESTEGO_ARCH "x86")
  endif()

  # MSVC Runtime
  if(MSVC_VERSION EQUAL 1400)
    set(IMAGESTEGO_RUNTIME "vc8")
  elseif(MSVC_VERSION EQUAL 1500)
    set(IMAGESTEGO_RUNTIME "vc9")
  elseif(MSVC_VERSION EQUAL 1600)
    set(IMAGESTEGO_RUNTIME "vc10")
  elseif(MSVC_VERSION EQUAL 1700)
    set(IMAGESTEGO_RUNTIME "vc11")
  elseif(MSVC_VERSION EQUAL 1800)
    set(IMAGESTEGO_RUNTIME "vc12")
  elseif(MSVC_VERSION EQUAL 1900)
    set(IMAGESTEGO_RUNTIME "vc14")
  elseif(MSVC_VERSION MATCHES "^191[0-9]$")
    set(IMAGESTEGO_RUNTIME "vc15")
  elseif(MSVC_VERSION MATCHES "^192[0-9]$")
    set(IMAGESTEGO_RUNTIME vc16)
  else()
    message(WARNING "Cannot recognize MSVC version")
  endif()
elseif(MINGW)
  set(IMAGESTEGO_RUNTIME "mingw")
  if (X86_64)
    set(IMAGESTEGO_ARCH "x64")
  else()
    set(IMAGESTEGO_ARCH "x86")
  endif()
endif()

include(TestBigEndian)
test_big_endian(IMAGESTEGO_BIG_ENDIAN)

# TODO: add c++11 test
