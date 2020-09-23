add_library(imagestego_compression
    ${CMAKE_CURRENT_SOURCE_DIR}/src/huffman_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/huffman_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_decoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_dictionary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lzw_encoder.cpp
)

set_target_properties(imagestego_compression PROPERTIES
    OUTPUT_NAME "imagestego_comp"
)

target_include_directories(imagestego_compression PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(imagestego_compression PUBLIC
    imagestego_core
    coverage_config
)

target_compile_definitions(imagestego_compression PUBLIC
    "-DIMAGESTEGO_COMPRESSION_SUPPORT"
)

imagestego_lib(imagestego_compression)
imagestego_exports(imagestego_compression)

install(TARGETS imagestego_compression
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/)

if (IMAGESTEGO_BUILD_TESTS)
    imagestego_test(COMPRESSION
        NAME huffman
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/huffman.cpp
        LIBS imagestego_compression
    )
    imagestego_test(COMPRESSION
        NAME lzw
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/lzw.cpp
        LIBS imagestego_compression
    )
endif(IMAGESTEGO_BUILD_TESTS)
