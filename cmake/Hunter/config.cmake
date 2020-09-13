hunter_config(
    Jpeg
    VERSION 9c-p0  
    CMAKE_ARGS BUILD_SHARED_LIBS=ON
)

hunter_config(
    OpenCV
    VERSION ${HUNTER_OpenCV_VERSION}
    CMAKE_ARGS BUILD_LIST=core,imgcodecs
    CMAKE_BUILD_TYPE=RELEASE
    BUILD_EXAMPLES=OFF
    BUILD_TESTS=OFF
)
