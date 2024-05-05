project(classes)


# General Files
set(SOURCES
    attak.cpp
)

add_library(classes STATIC ${SOURCES})

target_link_libraries(classes
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    core
    rust-rpg-bridge
)
