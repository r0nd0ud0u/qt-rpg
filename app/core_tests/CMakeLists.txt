cmake_minimum_required(VERSION 3.5)

project(core_tests)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(core_tests tst_utils.cpp)
add_test(NAME core_tests COMMAND core_tests)

target_link_libraries(core_tests PRIVATE core Qt${QT_VERSION_MAJOR}::Test)

