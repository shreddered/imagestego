
# imagestego_core
add_library(imagestego_core SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/avl_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
    # third party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/MurmurHash3.cpp
)

set_target_properties(imagestego_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY lib/
)

target_include_directories(imagestego_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
)

imagestego_lib(imagestego_core)

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
endif(IMAGESTEGO_BUILD_TESTS)
