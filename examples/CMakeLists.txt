file(GLOB IMAGESTEGO_EXAMPLES_SOURCES *.cpp)

foreach(IMAGESTEGO_EXAMPLE_SOURCE ${IMAGESTEGO_EXAMPLES_SOURCES})
  get_filename_component(IMAGESTEGO_EXAMPLE_NAME ${IMAGESTEGO_EXAMPLE_SOURCE} NAME_WE)
  add_executable(${IMAGESTEGO_EXAMPLE_NAME} ${IMAGESTEGO_EXAMPLE_SOURCE})
  target_link_libraries(${IMAGESTEGO_EXAMPLE_NAME} imagestego)
endforeach()
