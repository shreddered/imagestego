imagestego_defs()

add_library(imagestego_lossless
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lsb.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/route.cpp
)

target_include_directories(imagestego_lossless PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
  /usr/local/include/opencv4/
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
)

target_link_libraries(imagestego_lossless PUBLIC
  imagestego_core
  opencv_core
  opencv_imgcodecs
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

imagestego_add_test(LOSSLESS
  NAME lsb
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/lsb.cpp
  LIBS imagestego_lossless
)

imagestego_add_test(LOSSLESS
  NAME avl
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/avl.cpp
  LIBS imagestego_lossless
)
imagestego_add_test(LOSSLESS
  NAME route
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/route.cpp
  LIBS imagestego_lossless
)
