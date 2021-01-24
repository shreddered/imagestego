imagestego_defs()

imagestego_library(imagestego_wavelet
  ${CMAKE_CURRENT_SOURCE_DIR}/src/haar.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/haar.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/inverse_haar.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet.cpp
)

target_include_directories(imagestego_wavelet PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
)

target_link_libraries(imagestego_wavelet PUBLIC
  imagestego_core
  opencv_core
)

target_compile_definitions(imagestego_wavelet PUBLIC
  "-DIMAGESTEGO_WAVELET"
)

imagestego_add_perf_test(WAVELET
  NAME haar_perf
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/perf/haar.cpp
  LIBS imagestego_wavelet
)
