imagestego_defs()

# imagestego_core
imagestego_library(imagestego_core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bitarray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bitarrayimpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsic.cpp
  # third party
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
)

target_include_directories(imagestego_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:include/>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

imagestego_add_test(CORE
  NAME bitarray
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/bitarray.cpp
  LIBS imagestego_core
)
imagestego_add_test(CORE
  NAME intrinsics
  FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/intrinsic.cpp
  LIBS imagestego_core
)
