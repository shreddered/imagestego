cmake_minimum_required(VERSION 3.4)

project(imagestego VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -D_DEBUG")
string(APPEND CMAKE_CXX_FLAGS " -ggdb")

file(MAKE_DIRECTORY lib/)
file(MAKE_DIRECTORY bin/)
file(MAKE_DIRECTORY test/)

add_library(imagestego SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stego.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
)
target_include_directories(imagestego
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
    PRIVATE /usr/include/opencv4/
)
set_target_properties(imagestego PROPERTIES 
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)
target_link_libraries(imagestego 
    opencv_core
    opencv_imgcodecs
)

# tests
# avl
add_executable(test_avl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/avl.cpp
)
target_include_directories(test_avl
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
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
)
target_link_libraries(test_huffman
    imagestego
    opencv_core
    opencv_imgcodecs
)
set_target_properties(test_huffman PROPERTIES
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

# examples
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
target_include_directories(jstego
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE /usr/include/opencv4/
)
target_link_libraries(dwt
    imagestego
    opencv_core
    opencv_imgcodecs
    jpeg
)
set_target_properties(dwt PROPERTIES
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/
)

enable_testing()

add_test(NAME avl COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_avl)
add_test(NAME prefix COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_prefix)
add_test(NAME huffman COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_huffman)
add_test(NAME bitarray COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/test_bitarray)
