imagestego_defs()

add_library(imagestego_wavelet
  ${CMAKE_CURRENT_SOURCE_DIR}/src/haar.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lifting.c
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
  foreach(OPT SSE2 SSSE3 AVX2)
    if (CPU_${OPT}_SUPPORTED)
      target_compile_definitions(imagestego_wavelet PRIVATE
        "-D IMAGESTEGO_${OPT}_SUPPORTED=1"
      )
    else()
      target_compile_definitions(imagestego_wavelet PRIVATE
        "-D IMAGESTEGO_${OPT}_SUPPORTED=0"
      )
    endif()
  endforeach()
elseif(ARM OR AARCH64)
  if (CPU_NEON_SUPPORTED)
    target_compile_definitions(imagestego_wavelet PRIVATE
      "-D IMAGESTEGO_NEON_SUPPORTED=1"
    )
  else()
    target_compile_definitions(imagestego_wavelet PRIVATE
      "-D IMAGESTEGO_NEON_SUPPORTED=0"
    )
  endif()
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
