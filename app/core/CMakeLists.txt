project(core)


# General Files
set(HEADERS
    include/Application.h
    include/Stylizer.h
    include/Stylizertheme.h
    include/gamemanager.h
    include/character.h
    include/playersmanager.h
    include/stuff.h
    include/common.h
    include/effect.h

)
set(SOURCES
    src/Application.cpp
    src/Stylizer.cpp
    src/Stylizertheme.cpp
    src/gamemanager.cpp
    src/character.cpp
    src/stuff.cpp
    src/playersmanager.cpp
    src/bosses.cpp
    src/effect.cpp
)


add_library(core STATIC ${SOURCES} ${HEADERS})

target_include_directories(core PUBLIC include)

target_link_libraries(core
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)
