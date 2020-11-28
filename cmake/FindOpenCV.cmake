# CMake module for finding OpenCV.

find_path(OpenCV_INCLUDE_DIRS NAMES
  opencv2/core.hpp
  opencv2/imgcodecs.hpp
  HINTS "$ENV{OPENCV_DIR}/include"
)
if (WIN32)
  find_library(OpenCV_LIBRARIES NAMES
    opencv_core
    opencv_imgcodecs
    HINTS $ENV{OPENCV_DIR}/bin
  )
else()
  find_library(OpenCV_LIBRARIES NAMES
    opencv_core
    opencv_imgcodecs
    HINTS $ENV{OPENCV_DIR}/lib
  )
endif()

include(FindPackageHandleStandartArgs)
find_package_handle_standart_args(OpenCV DEFAULT_MSG OpenCV_LIBRARIES OpenCV_INCLUDE_DIRS)
mark_as_advanced(OpenCV_INCLUDE_DIRS OpenCV_LIBRARIES)
