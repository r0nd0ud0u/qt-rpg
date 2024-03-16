project(core_tests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    tst_utils.cpp
    tst_character.cpp
)

add_library(src_core_tests STATIC ${SOURCES})

target_link_libraries(src_core_tests PRIVATE core Qt${QT_VERSION_MAJOR}::Test)
