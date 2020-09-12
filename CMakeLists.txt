# TODO: JPEG module

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

project(imagestego VERSION 1.0 LANGUAGES CXX)

# build conditions
option(IMAGESTEGO_BUILD_TESTS "Build tests" ON)
option(IMAGESTEGO_BUILD_EXAMPLES "Build examples" ON)

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

find_package(OpenCV REQUIRED)

# libimagestego

add_library(imagestego_core SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    # third party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
)

target_include_directories(imagestego_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

list(APPEND IMAGESTEGO_LIBS imagestego_core)

if (IMAGESTEGO_LOSSLESS_FORMATS)
    add_library(imagestego_lossless SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imagestego.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
    )

    set_target_properties(imagestego_lossless PROPERTIES
        OUTPUT_NAME "imagestego_lossless"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    )

    target_link_libraries(imagestego_lossless
        PUBLIC imagestego_core
        opencv_core
        opencv_imgcodecs
    )

    target_include_directories(imagestego_lossless PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
    )

    list(APPEND IMAGESTEGO_LIBS imagestego_lossless)

    add_definitions("-DIMAGESTEGO_LOSSLESS_FORMATS")
endif(IMAGESTEGO_LOSSLESS_FORMATS)

if (IMAGESTEGO_JPEG_SUPPORT)
    add_library(imagestego_jpeg SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/f3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_lsb.cpp
        # third party
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
    )
    set_target_properties(imagestego_jpeg PROPERTIES
        OUTPUT_NAME "imagestego_jpeg"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    )

    target_link_libraries(imagestego_jpeg
        PUBLIC jpeg
        imagestego_core
    )

    target_include_directories(imagestego_jpeg PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
    )

    list(APPEND IMAGESTEGO_LIBS imagestego_jpeg)

    add_definitions("-DIMAGESTEGO_JPEG_SUPPORT")
endif(IMAGESTEGO_JPEG_SUPPORT)

# libimagestego_comp
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
    target_link_libraries(imagestego_compression
        PUBLIC imagestego_core
    )

    add_definitions("-DIMAGESTEGO_COMPRESSION_SUPPORT")
    target_include_directories(imagestego_compression PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
    )

    list(APPEND IMAGESTEGO_LIBS imagestego_compression)
endif(IMAGESTEGO_COMPRESSION_SUPPORT)

# macro used for generating tests
macro(generate_test)
    cmake_parse_arguments(TEST
        "OPTION"
        "NAME;FILE"
        "LIBS"
        ${ARGN}
    )
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} 
        ${IMAGESTEGO_LIBS}
        gtest_main
    )
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

    generate_test(NAME avl 
        FILE ${TEST_DIR}/avl.cpp
    )
    generate_test(NAME huffman
        FILE ${TEST_DIR}/huffman.cpp
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

    if (IMAGESTEGO_LOSSLESS_FORMATS)
        generate_example(NAME lsb SOURCES
            ${EXAMPLE_DIR}/lsb.cpp
        )
    endif(IMAGESTEGO_LOSSLESS_FORMATS)
endif(IMAGESTEGO_BUILD_EXAMPLES)

message(${IMAGESTEGO_LIBS})

#installation rules
install(TARGETS ${IMAGESTEGO_LIBS}
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib/
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/)
install(EXPORT imagestego-config DESTINATION cmake/)
