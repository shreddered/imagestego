# imagestego_core
add_library(imagestego_core SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bitarray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/intrinsic.cpp
    # third party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
)

target_include_directories(imagestego_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
    /usr/include/opencv4
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

imagestego_lib(imagestego_core)
imagestego_exports(imagestego_core)

target_link_libraries(imagestego_core PUBLIC coverage_config)

include(TestBigEndian)
test_big_endian(BIG_ENDIAN)

if (BIG_ENDIAN)
    target_compile_definitions(imagestego_core PUBLIC
        "IMAGESTEGO_BIG_ENDIAN"
    )
else()
    target_compile_definitions(imagestego_core PUBLIC
        "IMAGESTEGO_LITTLE_ENDIAN"
    )
endif(BIG_ENDIAN)

install(TARGETS imagestego_core
    EXPORT imagestego-config
    ARCHIVE DESTINATION lib/
    LIBRARY DESTINATION lib/
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/
)

if(IMAGESTEGO_BUILD_TESTS)
    imagestego_test(CORE 
        NAME avl
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/avl.cpp
        LIBS imagestego_core
    )
    imagestego_test(CORE
        NAME bitarray
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/test/bitarray.cpp
        LIBS imagestego_core
    )
endif(IMAGESTEGO_BUILD_TESTS)
