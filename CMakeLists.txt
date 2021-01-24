cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif (POLICY CMP0074)

if (POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif (POLICY CMP0042)

if (POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)
endif (POLICY CMP0091)

set(IMAGESTEGO_VERSION_MAJOR 1)
set(IMAGESTEGO_VERSION_MINOR 0)
set(IMAGESTEGO_VERSION_PATCH 0)
set(IMAGESTEGO_VERSION "${IMAGESTEGO_VERSION_MAJOR}.${IMAGESTEGO_VERSION_MINOR}.${IMAGESTEGO_VERSION_PATCH}")

project(imagestego VERSION ${IMAGESTEGO_VERSION} LANGUAGES C CXX)

# basic definitions
set(IMAGESTEGO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IMAGESTEGO_CORE_DIR ${IMAGESTEGO_SOURCE_DIR}/modules/core)
set(IMAGESTEGO_COMPRESSION_DIR ${IMAGESTEGO_SOURCE_DIR}/modules/compression)
set(IMAGESTEGO_LOSSLESS_DIR ${IMAGESTEGO_SOURCE_DIR}/modules/lossless)
set(IMAGESTEGO_WAVELET_DIR ${IMAGESTEGO_SOURCE_DIR}/modules/wavelet)

macro(imagestego_defs)
  set(IMAGESTEGO_SOURCE_DIR ${IMAGESTEGO_SOURCE_DIR})
  set(IMAGESTEGO_CORE_DIR ${IMAGESTEGO_CORE_DIR})
  set(IMAGESTEGO_COMPRESSION_DIR ${IMAGESTEGO_COMPRESSION_DIR})
  set(IMAGESTEGO_LOSSLESS_DIR ${IMAGESTEGO_LOSSLESS_DIR})
  set(IMAGESTEGO_WAVELET_DIR ${IMAGESTEGO_WAVELET_DIR})
endmacro()

# HW/Compiler features
include("${IMAGESTEGO_SOURCE_DIR}/cmake/imagestego-simd-detection.cmake")
include("${IMAGESTEGO_SOURCE_DIR}/cmake/imagestego-compiler-features.cmake")
if (DEFINED ENV{OPENCV_DIR})
  message(STATUS "Found OPENCV_DIR env variable: $ENV{OPENCV_DIR}")
  set(CMAKE_PREFIX_PATH $ENV{OPENCV_DIR}/cmake)
endif()
# opencv
find_package(OpenCV CONFIG REQUIRED)
# threads
find_package(Threads)

if (NOT DEFINED CMAKE_BUILD_TYPE)
  if (IMAGESTEGO_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug)
  else()
    set(CMAKE_BUILD_TYPE Release)
  endif()
endif()

if (NOT DEFINED CMAKE_CXX_FLAGS_RELEASE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0 -DIMAGESTEGO_DEBUG")
string(APPEND CMAKE_C_FLAGS_DEBUG " -O0 -DIMAGESTEGO_DEBUG")

if (IMAGESTEGO_GCC)
  string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic")
endif (IMAGESTEGO_GCC)

# build conditions
option(IMAGESTEGO_BUILD_TESTS "Build tests" OFF)
option(IMAGESTEGO_BUILD_PERF_TESTS "Build performance tests" ON)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)
option(IMAGESTEGO_INSTALL "Install imagestego" ON)
option(IMAGESTEGO_WITH_LIBJPEG "Build own libjpeg" OFF)
option(IMAGESTEGO_BUILD_OPENCV "Build own opencv" OFF)
option(IMAGESTEGO_COVERAGE "Check coverage" OFF)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)
option(IMAGESTEGO_BUILD_DOCS "Build docs" OFF)
set(IMAGESTEGO_MODULES "core,lossless,compression,wavelet" CACHE STRING "imagestego modules")

if (WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif (WIN32)

# function for test generation
macro(imagestego_add_test)
  cmake_parse_arguments(TEST
    "CORE;COMPRESSION;JPEG;LOSSLESS;WAVELET"
    "NAME;FILE"
    "LIBS"
    ${ARGN}
  )

  foreach(OPT CORE COMPRESSION JPEG LOSSLESS WAVELET)
    if (${TEST_${OPT}})
      set(DIR ${OPT})
      break()
    endif ()
  endforeach()

  list(APPEND IMAGESTEGO_TEST_FILES "${TEST_FILE}")
  list(APPEND IMAGESTEGO_TEST_LIBS "${TEST_LIBS}")
  list(APPEND IMAGESTEGO_TEST_INCLUDES "${IMAGESTEGO_${DIR}_DIR}/src")

  set(IMAGESTEGO_TEST_FILES "${IMAGESTEGO_TEST_FILES}" PARENT_SCOPE)
  set(IMAGESTEGO_TEST_LIBS "${IMAGESTEGO_TEST_LIBS}" PARENT_SCOPE)
  set(IMAGESTEGO_TEST_INCLUDES "${IMAGESTEGO_TEST_INCLUDES}" PARENT_SCOPE)

endmacro(imagestego_add_test)

# function for performance test generation
macro(imagestego_add_perf_test)
  cmake_parse_arguments(TEST
    "CORE;COMPRESSION;JPEG;LOSSLESS;WAVELET"
    "NAME;FILE"
    "LIBS"
    ${ARGN}
  )

  foreach(OPT CORE COMPRESSION JPEG LOSSLESS WAVELET)
    if (${TEST_${OPT}})
      set(DIR ${OPT})
    endif ()
  endforeach()

  list(APPEND IMAGESTEGO_PERF_TEST_FILES "${TEST_FILE}")
  list(APPEND IMAGESTEGO_PERF_TEST_LIBS "${TEST_LIBS}")
  list(APPEND IMAGESTEGO_PERF_TEST_INCLUDES "${IMAGESTEGO_${DIR}_DIR}/src")

  set(IMAGESTEGO_PERF_TEST_FILES "${IMAGESTEGO_PERF_TEST_FILES}" PARENT_SCOPE)
  set(IMAGESTEGO_PERF_TEST_LIBS "${IMAGESTEGO_PERF_TEST_LIBS}" PARENT_SCOPE)
  set(IMAGESTEGO_PERF_TEST_INCLUDES "${IMAGESTEGO_PERF_TEST_INCLUDES}" PARENT_SCOPE)
endmacro(imagestego_add_perf_test)

# helper function for defining export macros
function(imagestego_exports target)
  if (WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(${target} PUBLIC
      "-DIMAGESTEGO_DLL"
    )
    if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
      target_compile_definitions(${target} PRIVATE
        "-DIMAGESTEGO_EXPORT"
      )
    endif (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  endif (WIN32 AND BUILD_SHARED_LIBS)
endfunction(imagestego_exports target)

# macro for adding target to code coverage
macro(imagestego_add_coverage target)
  if (IMAGESTEGO_COVERAGE AND IMAGESTEGO_GCC)
    target_compile_options(${target} PRIVATE
      -g
      --coverage
    )
    if(NOT CMAKE_VERSION VERSION_LESS 3.13)
      target_link_options(${target} INTERFACE --coverage)
    else()
      target_link_libraries(${target} INTERFACE --coverage)
    endif ()
  endif (IMAGESTEGO_COVERAGE AND IMAGESTEGO_GCC)
endmacro()

string(REPLACE "," ";" IMAGESTEGO_BUILD_LIST "${IMAGESTEGO_MODULES}")

# imagestego interface
add_library(imagestego INTERFACE)

# installation rules
include("cmake/imagestego-install.cmake")

# macro for marking every imagestego module
macro(imagestego_library)
  if (ARGC EQUAL 0)
    message(FATAL_ERROR "Function imagestego_library() requires at least one argument")
  endif()

  list(APPEND IMAGESTEGO_LIBS ${ARGV0})
  set(IMAGESTEGO_LIBS ${IMAGESTEGO_LIBS} PARENT_SCOPE)

  if (IMAGESTEGO_INSTALL)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
      DESTINATION ${IMAGESTEGO_INCLUDE_DESTINATION}
    )
  endif()

  # library target
  if (ARGC EQUAL 1)
    add_library(${ARGV0} INTERFACE)
  else()
    string(REPLACE "${ARGV0};" "" ${ARGV0}_SOURCES "${ARGV}")
    # library target
    add_library(${ARGV0} ${${ARGV0}_SOURCES})

    target_link_libraries(imagestego INTERFACE ${ARGV0})
    # adding to coverage
    imagestego_add_coverage(${ARGV0})
    # compiler ID
    foreach(COMPILER GCC CLANG ICC MSVC)
      if (IMAGESTEGO_${COMPILER})
        target_compile_definitions(${ARGV0} PRIVATE
          "-D IMAGESTEGO_${COMPILER}=1"
        )
      else()
        target_compile_definitions(${ARGV0} PRIVATE
          "-D IMAGESTEGO_${COMPILER}=0"
        )
      endif()
    endforeach()
    if (Threads_FOUND)
      target_link_libraries(${ARGV0} PUBLIC Threads::Threads)
    endif()
    # Compiler and hardware features macro definitions
    # restrict
    target_compile_definitions(${ARGV0} PUBLIC
      "-D IMAGESTEGO_RESTRICT=${RESTRICT}"
    )
    # inline
    target_compile_definitions(${ARGV0} PUBLIC
      "-D IMAGESTEGO_INLINE=${INLINE}"
    )
    # constexpr
    target_compile_definitions(${ARGV0} PUBLIC
      "-D IMAGESTEGO_CONSTEXPR=constexpr"
    )
    # exports
    imagestego_exports(${ARGV0})
    # Endianness
    if (IMAGESTEGO_BIG_ENDIAN)
      target_compile_definitions(${ARGV0} PRIVATE
        "IMAGESTEGO_BIG_ENDIAN"
      )
    else()
      target_compile_definitions(${ARGV0} PRIVATE
        "IMAGESTEGO_LITTLE_ENDIAN"
      )
    endif(IMAGESTEGO_BIG_ENDIAN)
    # intrin.h
    if(HAVE_INTRIN_H)
      target_compile_definitions(imagestego_core PRIVATE
        "HAVE_INTRIN_H"
      )
    endif()
    # SIMD definitions
    if (X86 OR X86_64)
      foreach(OPT SSE2 SSSE3 AVX2 AVX512BW AVX512VL)
        if (CPU_${OPT}_SUPPORTED)
          target_compile_definitions(${ARGV0} PRIVATE
            "-D IMAGESTEGO_${OPT}_SUPPORTED=1"
          )
        else()
          target_compile_definitions(${ARGV0} PRIVATE
            "-D IMAGESTEGO_${OPT}_SUPPORTED=0"
          )
        endif()
      endforeach()
    elseif(ARM OR AARCH64)
      if (CPU_NEON_SUPPORTED)
        target_compile_definitions(${ARGV0} PRIVATE
          "-D IMAGESTEGO_NEON_SUPPORTED=1"
        )
      else()
        target_compile_definitions(${ARGV0} PRIVATE
          "-D IMAGESTEGO_NEON_SUPPORTED=0"
        )
      endif()
    elseif(PPC64 OR PPC64LE)
      # TODO: check for altivec support
      target_compile_definitions(${ARGV0} PRIVATE
        "-DIMAGESTEGO_ALTIVEC_SUPPORTED"
      )
    endif(X86 OR X86_64)
  endif()
endmacro()

# preparing needed directories
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)
  file(MAKE_DIRECTORY test/)
endif (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)

# print summary of all options
message(STATUS "Build configuration for ${PROJECT_NAME} ${IMAGESTEGO_VERSION}:")
message(STATUS "\tPlatform:")
message(STATUS "\t\tHost:                ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "\t\tCMake:               ${CMAKE_VERSION}")
message(STATUS "\t\tC compiler:          ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "\t\tC++ compiler:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS)
string(REPLACE ";" " " CPU_SUPPORTED_EXTENSIONS "${CPU_SUPPORTED_EXTENSIONS}")
message(STATUS "\tCPU features:\t ${CPU_SUPPORTED_EXTENSIONS}")
message(STATUS)
message(STATUS "\tC/C++:")
message(STATUS "\t\tShared libs:         ${BUILD_SHARED_LIBS}")
message(STATUS "\t\tC++ flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "\t\tC flags (Release):   ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "\t\tC++ flags (Debug):   ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "\t\tC flags (Debug):     ${CMAKE_C_FLAGS_DEBUG}")
string(REPLACE ";" ", " IMAGESTEGO_MODULES_STRING "${IMAGESTEGO_BUILD_LIST}")
message(STATUS "\tTo be built:\t         ${IMAGESTEGO_MODULES_STRING}")

foreach(OPT ${IMAGESTEGO_BUILD_LIST})
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modules/${OPT}")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/modules/${OPT}")
  else()
    message(WARNING "Unknown module: ${OPT}")
  endif()
endforeach()

if (IMAGESTEGO_BUILD_EXAMPLES)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs)

if (IMAGESTEGO_BUILD_TESTS OR IMAGESTEGO_BUILD_PERF_TESTS)
  enable_testing()
  # gtest
  set (INSTALL_GTEST OFF) # do not install GTest
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest)
endif()

# unit tests
if (IMAGESTEGO_BUILD_TESTS)
  # test executable
  add_executable(imagestego_test
    ${IMAGESTEGO_TEST_FILES}
  )
  target_link_libraries(imagestego_test
    ${IMAGESTEGO_TEST_LIBS}
    gtest_main
    gmock_main
  )
  target_include_directories(imagestego_test PRIVATE ${IMAGESTEGO_TEST_INCLUDES})

  add_test(NAME unit_tests COMMAND imagestego_test)
endif (IMAGESTEGO_BUILD_TESTS)

if (IMAGESTEGO_BUILD_PERF_TESTS)
  # test executable
  add_executable(imagestego_perf_test ${IMAGESTEGO_PERF_TEST_FILES})
  target_link_libraries(imagestego_perf_test
    ${IMAGESTEGO_PERF_TEST_LIBS}
    gtest_main
  )
  target_include_directories(imagestego_perf_test PRIVATE ${IMAGESTEGO_PERF_TEST_INCLUDES})

  add_test(NAME perf_test COMMAND imagestego_perf_test)
endif(IMAGESTEGO_BUILD_PERF_TESTS)

include("cmake/CPackConfig.cmake")

if (IMAGESTEGO_INSTALL)
  install(TARGETS ${IMAGESTEGO_LIBS}
    EXPORT imagestegoConfig
    INCLUDES DESTINATION ${IMAGESTEGO_INCLUDE_DESTINATION}
    ARCHIVE DESTINATION ${IMAGESTEGO_ARCHIVE_DESTINATION}
    LIBRARY DESTINATION ${IMAGESTEGO_LIBRARY_DESTINATION}
  )
  install(EXPORT imagestegoConfig
    NAMESPACE imagestego::
    DESTINATION ${IMAGESTEGO_CONFIG_DESTINATION}
  )
endif()
