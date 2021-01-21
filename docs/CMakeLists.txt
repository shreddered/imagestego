if (NOT IMAGESTEGO_BUILD_DOCS)
  return()
endif()

find_package(Doxygen)

if (DOXYGEN_FOUND)
  set(DOXYGEN_INPUT
    ${IMAGESTEGO_SOURCE_DIR}/modules
    ${IMAGESTEGO_SOURCE_DIR}/include
    ${IMAGESTEGO_SOURCE_DIR}/src
  )
  configure_file(${IMAGESTEGO_SOURCE_DIR}/docs/Doxyfile.in
    ${IMAGESTEGO_SOURCE_DIR}/docs/Doxyfile
  )
  add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE}
    ${IMAGESTEGO_SOURCE_DIR}/docs/Doxyfile
  )
else()
  message(WARNING "Cannot found doxygen - docs won't be created!")
endif()
