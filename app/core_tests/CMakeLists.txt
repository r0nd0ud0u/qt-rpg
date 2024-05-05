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

add_subdirectory(classes)

add_executable(tst_utils tst_utils.cpp)
add_executable(tst_characters tst_characters.cpp)
add_executable(tst_players_manager tst_players_manager.cpp)
add_executable(tst_game_manager tst_game_manager.cpp)

add_test(NAME tst_utils COMMAND tst_utils)
add_test(NAME tst_characters COMMAND tst_characters)
add_test(NAME tst_players_manager COMMAND tst_players_manager)
add_test(NAME tst_game_manager COMMAND tst_game_manager)

target_link_libraries(tst_utils PRIVATE core Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(tst_characters PRIVATE core Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(tst_players_manager PRIVATE core Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(tst_game_manager PRIVATE core Qt${QT_VERSION_MAJOR}::Test)

