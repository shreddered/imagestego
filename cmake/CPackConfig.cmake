include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_VENDOR "imagestego team")
set(CPACK_PACKAGE_CONTACT "x.shreddered.x@gmail.com")

set(CPACK_PACKAGE_VERSION_MAJOR ${IMAGESTEGO_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${IMAGESTEGO_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${IMAGESTEGO_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${IMAGESTEGO_VERSION})

set(CPACK_PACKAGE_DESCRIPTION "Image Steganography library written in C++ with data compression and wavelet transform support")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ Image Steganography library")

set(CPACK_PACKAGE_NAME "imagestego")
if (NOT WIN32)
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${IMAGESTEGO_ARCH}")
else()
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${IMAGESTEGO_RUNTIME}-${IMAGESTEGO_ARCH}")
endif()

set(CPACK_SOURCE_GENERATOR TGZ ZIP)

include(CPack)
