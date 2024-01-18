cmake_minimum_required(VERSION 3.5)

project(qt-rpg VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network Sql)

#include(${CMAKE_SOURCE_DIR}/build_conan/${CMAKE_BUILD_TYPE}/conanbuildinfo.cmake)
#conan_basic_setup()

add_subdirectory(app)

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/resources/resource.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-rpg
        MANUAL_FINALIZATION
        app/core/src/main.cpp
        ${RESOURCES}
    )
else()
    if(ANDROID)
        add_library(qt-rpg SHARED
            app/core/src/main.cpp
            ${RESOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt-rpg
            app/core/src/main.cpp
            ${RESOURCES}
        )
    endif()
endif()

target_link_libraries(qt-rpg PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    view
)



set_target_properties(qt-rpg PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-rpg)
endif()
