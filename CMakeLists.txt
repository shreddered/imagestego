cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

project(imagestego VERSION 1.0 LANGUAGES CXX)

# build conditions
option(IMAGESTEGO_BUILD_TESTS "Build tests" ON)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)
option(IMAGESTEGO_BUILD_TYPE DEBUG)
option(IMAGESTEGO_COMPRESSION_SUPPORT "Data compression support" ON)

if (IMAGESTEGO_BUILD_TYPE STREQUAL DEBUG)
    add_definitions("-DIMAGESTEGO_DEBUG")
endif(IMAGESTEGO_BUILD_TYPE STREQUAL DEBUG)

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

find_package(OpenCV REQUIRED)

# libimagestego
set(IMAGESTEGO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/f3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imagestego.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_lsb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
    # third party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
)

add_library(imagestego SHARED
    ${IMAGESTEGO_SOURCES}
)

set_target_properties(imagestego PROPERTIES
    OUTPUT_NAME "imagestego"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)

target_link_libraries(imagestego
    PUBLIC opencv_core
    PUBLIC opencv_imgcodecs
    PUBLIC jpeg
)

target_include_directories(imagestego
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

target_include_directories(imagestego INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/imagestego/algorithms/:include/imagestego/algorithms.hpp>
)

if (IMAGESTEGO_COMPRESSION_SUPPORT)
    add_library(imagestego_compression SHARED
        # Huffman encoding 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/huffman_decoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/huffman_encoder.cpp
        # LZW encoding
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_decoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_dictionary.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_encoder.cpp
    ) 
    set_target_properties(imagestego_compression PROPERTIES
        OUTPUT_NAME "imagestego_comp"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    )

    target_compile_definitions(imagestego_compression
        PUBLIC "IMAGESTEGO_COMPRESSION_SUPPORT"
    )
    target_include_directories(imagestego_compression INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/imagestego/compression/:include/imagestego/compression.hpp>
    )
    target_include_directories(imagestego_compression
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif(IMAGESTEGO_COMPRESSION_SUPPORT)

# macro used for generating tests
macro(generate_test)
    cmake_parse_arguments(TEST
        "OPTION"
        "NAME"
        "SOURCES"
        ${ARGN}
    )
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    target_link_libraries(${TEST_NAME}
        imagestego
        gtest_main
    )
    if (TARGET imagestego_compression)
        target_link_libraries(${TEST_NAME}
            imagestego_compression
        )
    endif(TARGET imagestego_compression)
    set_target_properties(${TEST_NAME} PROPERTIES
        OUTPUT_NAME "test_${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/test/
    )
    add_test(NAME ${TEST_NAME} COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bin/test/test_${TEST_NAME}")
endmacro()

# tests
if (IMAGESTEGO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest/)

    # should set TEST_DIR
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/)

    generate_test(NAME avl SOURCES
        ${TEST_DIR}/avl.cpp
    )
    generate_test(NAME huffman SOURCES
        ${TEST_DIR}/huffman.cpp
    )
endif(IMAGESTEGO_BUILD_TESTS)

# macro used for building examples
macro(generate_example)
    cmake_parse_arguments(EXAMPLE
        "OPTION"
        "NAME"
        "SOURCES"
        ${ARGN}
    )
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME}
        imagestego
    )
    if (TARGET imagestego_compression)
        target_link_libraries(${EXAMPLE_NAME}
            imagestego_compression
        )
    endif(TARGET imagestego_compression)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    )
endmacro()

# examples
if (IMAGESTEGO_BUILD_EXAMPLES)
    set(EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
    generate_example(NAME lsb SOURCES
        ${EXAMPLE_DIR}/lsb.cpp
    )
endif(IMAGESTEGO_BUILD_EXAMPLES)
