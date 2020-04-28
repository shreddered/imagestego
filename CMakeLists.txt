cmake_minimum_required(VERSION 3.4)

project(imagestego VERSION 1.0)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -D_DEBUG")

file(MAKE_DIRECTORY lib/)

add_library(imagestego SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hufman_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb_stego.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stego.cpp
)

set_target_properties(imagestego PROPERTIES 
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/lib/
)

target_link_libraries(imagestego opencv_core)
target_link_libraries(imagestego opencv_imgcodecs)

add_executable(test_avl
    ${CMAKE_CURRENT_SOURCE_DIR}/test/avl.cpp
)

add_executable(test_prefix
    ${CMAKE_CURRENT_SOURCE_DIR}/test/prefix.cpp
)

add_executable(test_huffman
    ${CMAKE_CURRENT_SOURCE_DIR}/test/huffman.cpp
)

target_include_directories(imagestego
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
    PRIVATE /usr/include/opencv4
)

target_include_directories(test_avl
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_include_directories(test_prefix
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_include_directories(test_huffman
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(test_avl imagestego)

target_link_libraries(test_prefix imagestego)

target_link_libraries(test_huffman imagestego)
target_link_libraries(test_huffman opencv_imgcodecs)
target_link_libraries(test_huffman opencv_core)

enable_testing()

add_test(NAME avl COMMAND test_avl)
add_test(NAME prefix COMMAND test_prefix)
add_test(NAME huffman COMMAND test_huffman)
