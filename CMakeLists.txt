cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif (POLICY CMP0074)

if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif (POLICY CMP0042)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic")
endif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

if (POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif (POLICY CMP0091)

project(imagestego VERSION 1.0 LANGUAGES CXX)

set(IMAGESTEGO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include("${IMAGESTEGO_SOURCE_DIR}/cmake/imagestego-simd-detection.cmake")

# build conditions
option(IMAGESTEGO_BUILD_TESTS "Build tests" OFF)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)
option(IMAGESTEGO_WITH_LIBJPEG "Build own libjpeg" OFF)
option(IMAGESTEGO_BUILD_OPENCV "Build own opencv" OFF)
option(IMAGESTEGO_COVERAGE "Check coverage" OFF)

# modules
option(IMAGESTEGO_MODULES "imagestego modules" "core,jpeg,lossless,comp")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DIMAGESTEGO_DEBUG")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif (WIN32)

# preparing needed directories
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    file(MAKE_DIRECTORY bin/)
endif (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)
    file(MAKE_DIRECTORY test/)
endif (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)

# important
macro(imagestego_lib)
    list(APPEND IMAGESTEGO_LIBS ${ARGV})
endmacro()

add_library(coverage_config INTERFACE)

if (IMAGESTEGO_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(coverage_config INTERFACE
        -O0
        -g
        --coverage
    )
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(coverage_config INTERFACE --coverage)
    else()
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif ()
endif (IMAGESTEGO_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")

# function for test generation
function(imagestego_test)
    cmake_parse_arguments(TEST
        "CORE;COMPRESSION;JPEG;LOSSLESS;WAVELET"
        "NAME;FILE"
        "LIBS"
        ${ARGN}
    )

    foreach(OPT CORE COMPRESSION JPEG LOSSLESS WAVELET)
        if (${TEST_${OPT}})
            set(DIR $<LOWER_CASE:${OPT}>)
        endif ()
    endforeach()

    # test executable
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME}
        ${TEST_LIBS}
        gtest_main
        gmock_main
    )

    set_target_properties(${TEST_NAME} PROPERTIES
        OUTPUT_NAME "test_${DIR}_${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
    )
endfunction(imagestego_test)

function(imagestego_exports target)
    if (WIN32 AND BUILD_SHARED_LIBS)
        target_compile_definitions(${target} PUBLIC
            "-DIMAGESTEGO_DLL"
        )
        if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
            target_compile_definitions(${target} PRIVATE
                "-DIMAGESTEGO_EXPORTS"
            )
        endif (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    endif (WIN32 AND BUILD_SHARED_LIBS)
endfunction(imagestego_exports target)

string(REPLACE "," ";" IMAGESTEGO_BUILD_LIST "${IMAGESTEGO_MODULES}")

add_library(imagestego INTERFACE)

target_link_libraries(imagestego INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

# core module
add_subdirectory("modules/core/")
target_link_libraries(imagestego INTERFACE imagestego_core)

# compression module
if ("comp" IN_LIST IMAGESTEGO_BUILD_LIST)
    add_subdirectory("modules/compression")
    target_link_libraries(imagestego INTERFACE imagestego_compression)
endif ("comp" IN_LIST IMAGESTEGO_BUILD_LIST)

# lossless module
if ("lossless" IN_LIST IMAGESTEGO_BUILD_LIST)
    add_subdirectory("modules/lossless")
    target_link_libraries(imagestego INTERFACE imagestego_lossless)
endif ("lossless" IN_LIST IMAGESTEGO_BUILD_LIST)

# jpeg module
if ("jpeg" IN_LIST IMAGESTEGO_BUILD_LIST)
    add_subdirectory("modules/jpeg")
    target_link_libraries(imagestego INTERFACE imagestego_jpeg)
endif ("jpeg" IN_LIST IMAGESTEGO_BUILD_LIST)

if ("wavelet" IN_LIST IMAGESTEGO_BUILD_LIST)
    add_subdirectory("modules/wavelet")
endif ("wavelet" IN_LIST IMAGESTEGO_BUILD_LIST)

function(imagestego_add_test)
    cmake_parse_arguments(TEST
        "CORE;COMPRESSION;JPEG;LOSSLESS;WAVELET"
        "NAME"
        ""
        ${ARGN}
    )

    foreach(OPT CORE COMPRESSION JPEG LOSSLESS WAVELET)
        if (${TEST_${OPT}})
            set(DIR $<LOWER_CASE:${OPT}>)
        endif ()
    endforeach()

    add_test(NAME ${TEST_NAME}
        COMMAND "${PROJECT_SOURCE_DIR}/bin/test_${DIR}_${TEST_NAME}"
    )
endfunction(imagestego_add_test)

if (IMAGESTEGO_BUILD_TESTS)
    include(CTest)

    # gtest
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest)

    # core tests
    imagestego_add_test(CORE NAME avl)
    imagestego_add_test(CORE NAME bitarray)
    imagestego_add_test(CORE NAME intrinsics)
    imagestego_add_test(CORE NAME route)

    # compression tests
    if ("comp" IN_LIST IMAGESTEGO_BUILD_LIST)
        imagestego_add_test(COMPRESSION NAME huffman)
        imagestego_add_test(COMPRESSION NAME lzw)
    endif ("comp" IN_LIST IMAGESTEGO_BUILD_LIST)

    # lossless tests
    if ("lossless" IN_LIST IMAGESTEGO_BUILD_LIST)
        imagestego_add_test(LOSSLESS NAME lsb)
    endif ("lossless" IN_LIST IMAGESTEGO_BUILD_LIST)

    # wavelet tests
    if ("wavelet" IN_LIST IMAGESTEGO_BUILD_LIST)
        imagestego_add_test(WAVELET NAME haar)
    endif ("wavelet" IN_LIST IMAGESTEGO_BUILD_LIST)
endif (IMAGESTEGO_BUILD_TESTS)
