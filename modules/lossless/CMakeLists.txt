add_library(imagestego_lossless SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dwt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
)

set_target_properties(imagestego_lossless PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY lib/
)

target_include_directories(imagestego_lossless PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(imagestego_lossless PUBLIC
    imagestego_core
    opencv_core
    opencv_imgcodecs
)

target_compile_definitions(imagestego_lossless PUBLIC
    "-DIMAGESTEGO_LOSSLESS_FORMATS"
)

imagestego_lib(imagestego_lossless)

install(TARGETS imagestego_lossless
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/)
