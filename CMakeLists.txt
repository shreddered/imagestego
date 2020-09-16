cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif(POLICY CMP0074)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"
    SHA1 "64024b7b95b4c86d50ae05b926814448c93a70a0"
    FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Hunter/config.cmake"
)

set(HUNTER_CONFIGURATION_TYPES Release)

if (CMAKE_COMPILER_IS_GNUCXX)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)
endif(POLICY CMP0091)

project(imagestego VERSION 1.0)

# build conditions
option(IMAGESTEGO_BUILD_TESTS "Build tests" OFF)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)
option(IMAGESTEGO_WITH_LIBJPEG "Build own libjpeg" OFF)
option(IMAGESTEGO_BUILD_OPENCV "Build own opencv" OFF)

# modules
option(IMAGESTEGO_COMPRESSION_SUPPORT "Data compression support" ON)
option(IMAGESTEGO_JPEG_SUPPORT "Enable JPEG steganography" ON)
option(IMAGESTEGO_LOSSLESS_FORMATS "Enable lossless steganography" ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DIMAGESTEGO_DEBUG")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# preparing needed directories
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
    file(MAKE_DIRECTORY lib/)
endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    file(MAKE_DIRECTORY bin/)
endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)
    file(MAKE_DIRECTORY test/)
endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)

# important
macro(imagestego_lib)
    list(APPEND IMAGESTEGO_LIBS ${ARGV})
endmacro()

# function for test generation
function(imagestego_test)
    cmake_parse_arguments(TEST
        "CORE;COMPRESSION;JPEG;LOSSLESS"
        "NAME;FILE"
        "LIBS"
        ${ARGN}
    )
    
    foreach(OPT CORE COMPRESSION JPEG LOSSLESS)
        if (${TEST_${OPT}})
            set(DIR $<LOWER_CASE:${OPT}>)
        endif()
    endforeach()
    
    # test executable
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} 
        ${TEST_LIBS}
        gtest_main
    )
    set_target_properties(${TEST_NAME} PROPERTIES
        OUTPUT_NAME "test_${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/test/${DIR}"
    )
endfunction(imagestego_test)

function(imagestego_exports target)
    target_compile_definitions(${target} PRIVATE
        "-DIMAGESTEGO_EXPORT"
    )
endfunction(imagestego_exports target)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# libjpeg
if (IMAGESTEGO_JPEG_SUPPORT AND IMAGESTEGO_WITH_LIBJPEG)
    message("Fetching imagestego_jpeg dependency - libjpeg")

    hunter_add_package(Jpeg)
    find_package(JPEG CONFIG REQUIRED)
endif(IMAGESTEGO_JPEG_SUPPORT AND IMAGESTEGO_WITH_LIBJPEG)

# opencv
if (IMAGESTEGO_LOSSLESS_FORMATS)
    if (IMAGESTEGO_BUILD_OPENCV)
        message("Fetching imagestego_lossless dependency - opencv")

        hunter_add_package(OpenCV)
    endif(IMAGESTEGO_BUILD_OPENCV)

    find_package(OpenCV REQUIRED)
endif(IMAGESTEGO_LOSSLESS_FORMATS)

# core module
add_subdirectory("modules/core/")

# compression module
if (IMAGESTEGO_COMPRESSION_SUPPORT)
    add_subdirectory("modules/compression")
endif(IMAGESTEGO_COMPRESSION_SUPPORT)

# lossless module
if (IMAGESTEGO_LOSSLESS_FORMATS)
    add_subdirectory("modules/lossless")
endif(IMAGESTEGO_LOSSLESS_FORMATS)

# jpeg module
if (IMAGESTEGO_JPEG_SUPPORT)
    add_subdirectory("modules/jpeg")
endif(IMAGESTEGO_JPEG_SUPPORT)

function(imagestego_add_test)
    cmake_parse_arguments(TEST
        "CORE;COMPRESSION;JPEG;LOSSLESS"
        "NAME"
        ""
        ${ARGN}
    )

    foreach(OPT CORE COMPRESSION JPEG LOSSLESS)
        if (${TEST_${OPT}})
            set(DIR $<LOWER_CASE:${OPT}>)
        endif()
    endforeach()

    add_test(NAME ${TEST_NAME}
        COMMAND "${PROJECT_SOURCE_DIR}/bin/test/${DIR}/test_${TEST_NAME}"
    )
endfunction(imagestego_add_test)

if (IMAGESTEGO_BUILD_TESTS)
    enable_testing()

    # gtest
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest)

    # core tests
    imagestego_add_test(CORE NAME avl)

    # compression tests
    imagestego_add_test(COMPRESSION NAME huffman)
endif(IMAGESTEGO_BUILD_TESTS)
