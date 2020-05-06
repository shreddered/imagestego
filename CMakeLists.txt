cmake_minimum_required(VERSION 3.4)

project(imagestego VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build conditions (default)
if (NOT DEFINED ENABLE_FORMAT_CHECKING)
    set(ENABLE_FORMAT_CHECKING ON)
endif()

if (NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE DEBUG)
endif()

if (NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS ON)
endif()

if (NOT DEFINED BUILD_EXAMPLES)
    set(BUILD_EXAMPLES ON)
endif()

if (NOT DEFINED ENABLE_KEYGEN_SUPPORT)
    set(ENABLE_KEYGEN_SUPPORT ON)
endif()

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

if (NOT BUILD_TYPE STREQUAL RELEASE)
    string(APPEND CMAKE_CXX_FLAGS " -D_DEBUG")
    string(APPEND CMAKE_CXX_FLAGS " -ggdb")
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
    file(MAKE_DIRECTORY lib/)
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    file(MAKE_DIRECTORY bin/)
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/)
    file(MAKE_DIRECTORY test/)
endif()

#################################################
#           libimagestego sources
#################################################

set(IMAGESTEGO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imagestego.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_lsb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp

    # 3rd party
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/MurmurHash3.cpp
)

if (ENABLE_FORMAT_CHECKING)
    set(IMAGESTEGO_SOURCES ${IMAGESTEGO_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/format_checker.cpp
    )
endif()

if (ENABLE_KEYGEN_SUPPORT)
    set(IMAGESTEGO_SOURCES ${IMAGESTEGO_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/keygen.cpp
    )
endif()

#################################################
#               libimagestego
#################################################

add_library(imagestego SHARED
    ${IMAGESTEGO_SOURCES}
)

target_include_directories(imagestego
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
    PRIVATE /usr/include/opencv4/ 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
)

set_target_properties(imagestego 
    PROPERTIES 
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    OUTPUT_NAME "libimagestego"
)

target_link_libraries(imagestego 
    opencv_core
    opencv_imgcodecs
    jpeg
)

if (ENABLE_FORMAT_CHECKING)
    target_compile_definitions(imagestego
        PRIVATE ENABLE_FORMAT_CHECKING
    )
endif()

if (ENABLE_KEYGEN_SUPPORT)
    target_compile_definitions(imagestego
        PRIVATE ENABLE_KEYGEN_SUPPORT
    )
endif()

#################################################
#               tests
#################################################

if (BUILD_TESTS)
    # avl
    add_executable(test_avl
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test/avl.cpp
    )

    target_include_directories(test_avl
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

    target_link_libraries(test_avl
        imagestego
    )

    set_target_properties(test_avl PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/test/
    )

    # bitarray
    add_executable(test_bitarray
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test/bitarray.cpp
    )

    target_include_directories(test_bitarray
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

    set_target_properties(test_bitarray PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/test/
    )

    # prefix
    add_executable(test_prefix
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test/prefix.cpp
    )
    target_include_directories(test_prefix
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )
    target_link_libraries(test_prefix
        imagestego
    )
    set_target_properties(test_prefix PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/test/
    )

    # huffman
    add_executable(test_huffman
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test/huffman.cpp
    )
    target_include_directories(test_huffman
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )
    target_link_libraries(test_huffman
        imagestego
        opencv_core
        opencv_imgcodecs
    )
    set_target_properties(test_huffman PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/test/
    )
endif()

#################################################
#               examples
#################################################

if (BUILD_EXAMPLES)
    # lsb
    add_executable(lsb
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/lsb.cpp
    )

    target_include_directories(lsb
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE /usr/include/opencv4/
    )

    target_link_libraries(lsb
        imagestego
        opencv_core
        opencv_imgcodecs
    )

    set_target_properties(lsb PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    )

    # stego
    add_executable(stego
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/archived_data_stego.cpp
    )

    target_include_directories(stego
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE /usr/include/opencv4/
    )

    target_link_libraries(stego
        imagestego
        opencv_core
        opencv_imgcodecs
    )

    set_target_properties(stego PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    )

    # jstego
    add_executable(jstego
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/jsteg.cpp
    )

    target_include_directories(jstego
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE /usr/include/opencv4/
    )

    target_link_libraries(jstego
        imagestego
        opencv_core
        opencv_imgcodecs
        jpeg
    )

    set_target_properties(jstego PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    )

    # dwt
    add_executable(dwt
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/dwt.cpp
    )

    target_include_directories(dwt
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE /usr/include/opencv4/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/
    )

    target_link_libraries(dwt
        imagestego
        opencv_core
        opencv_imgcodecs
    )

    set_target_properties(dwt PROPERTIES
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    )
endif()

if (BUILD_TESTS)
    enable_testing()

    add_test(NAME avl COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_avl)

    add_test(NAME prefix COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_prefix)

    add_test(NAME huffman COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_huffman)

    add_test(NAME bitarray COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_bitarray)
endif()

#################################################
#           installation rules
#################################################

if (NOT DEFINED IMAGESTEGO_PREFIX)
    set(IMAGESTEGO_PREFIX "/usr/lib/")
endif()

install(TARGETS imagestego DESTINATION ${IMAGESTEGO_PREFIX})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION /usr/include/)
