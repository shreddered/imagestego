imagestego_defs()

add_library(imagestego_wavelet
  ${CMAKE_CURRENT_SOURCE_DIR}/src/haar.cpp
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

if (X86 OR X86_64)
  foreach(OPT SSSE3 AVX2)
    if (CPU_${OPT}_SUPPORTED)
      target_compile_definitions(imagestego_wavelet PRIVATE
        "-DIMAGESTEGO_${OPT}_SUPPORTED"
      )
    endif()
  endforeach()
elseif(ARM OR AARCH64)
  target_compile_definitions(imagestego_wavelet PRIVATE
    "-DIMAGESTEGO_NEON_SUPPORTED"
  )
elseif(PPC64 OR PPC64LE)
  target_compile_definitions(imagestego_wavelet PRIVATE
    "-DIMAGESTEGO_ALTIVEC_SUPPORTED"
  )
endif(X86 OR X86_64)

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
