add_library(imagestego_wavelet
    ${CMAKE_CURRENT_SOURCE_DIR}/src/haar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/haar_fast.cpp
)

target_include_directories(imagestego_wavelet PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
    /usr/local/include/opencv4/
)

target_link_libraries(imagestego_wavelet PUBLIC
    imagestego_core
    opencv_core
    opencv_imgcodecs
    coverage_config
)

target_compile_definitions(imagestego_wavelet PUBLIC
    "-DIMAGESTEGO_WAVELET"
)

if (CPU_AVX2_SUPPORTED)
    target_compile_definitions(imagestego_wavelet PRIVATE
        "-DIMAGESTEGO_AVX2_SUPPORTED"
    )
endif()
if (CPU_SSSE3_SUPPORTED)
    target_compile_definitions(imagestego_wavelet PRIVATE
        "-DIMAGESTEGO_SSSE3_SUPPORTED"
    )
endif()

imagestego_lib(imagestego_wavelet)
imagestego_exports(imagestego_wavelet)

install(TARGETS imagestego_wavelet
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

if (IMAGESTEGO_BUILD_TESTS)
    imagestego_test(WAVELET
        NAME haar
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/haar.cpp
        LIBS imagestego_wavelet opencv_imgcodecs
    )
endif (IMAGESTEGO_BUILD_TESTS)
