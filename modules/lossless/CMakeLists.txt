add_library(imagestego_lossless
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
)

target_include_directories(imagestego_lossless PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(imagestego_lossless PUBLIC
    imagestego_core
    opencv_core
    opencv_imgcodecs
    coverage_config
)

if (TARGET imagestego_compression)
    target_link_libraries(imagestego_lossless PUBLIC
        imagestego_compression
    )
endif (TARGET imagestego_compression)

target_compile_definitions(imagestego_lossless PUBLIC
    "-DIMAGESTEGO_LOSSLESS_FORMATS"
)

imagestego_lib(imagestego_lossless)
imagestego_exports(imagestego_lossless)

install(TARGETS imagestego_lossless
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/)

if (IMAGESTEGO_BUILD_TESTS)
    imagestego_test(LOSSLESS
        NAME lsb
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/lsb.cpp
        LIBS imagestego_lossless
    )
endif (IMAGESTEGO_BUILD_TESTS)
