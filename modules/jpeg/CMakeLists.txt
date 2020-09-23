add_library(imagestego_jpeg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/f3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jpeg_lsb.cpp
)

target_include_directories(imagestego_jpeg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)


target_link_libraries(imagestego_jpeg PUBLIC
    imagestego_core
    $<$<TARGET_EXISTS:JPEG::jpeg>:JPEG::jpeg>
    $<$<NOT:$<TARGET_EXISTS:JPEG::jpeg>>:jpeg>
)

target_compile_definitions(imagestego_jpeg PUBLIC
    "-DIMAGESTEGO_JPEG_SUPPORT"
)

imagestego_lib(imagestego_jpeg)
imagestego_exports(imagestego_jpeg)

install(TARGETS imagestego_jpeg
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/)
