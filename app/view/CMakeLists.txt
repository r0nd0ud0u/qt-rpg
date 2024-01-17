project(view)

# Qt pre-compilation activation
set(CMAKE_AUTOUIC_SEARCH_PATHS ./forms)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
        src/mainwindow.cpp
        src/gamedisplay.cpp
        src/heroesview.cpp
        src/hostpage.cpp
        src/heropanel.cpp
        src/stuffpanel.cpp
        src/stuffsview.cpp
        src/channel.cpp
        src/CompButtons.cpp
        src/bossesview.cpp
        src/bosspanel.cpp
        src/statsview.cpp
)

set(HEADERS
        include/mainwindow.h
        include/gamedisplay.h
        include/heroesview.h
        include/hostpage.h
        include/heropanel.h
        include/stuffpanel.h
        include/stuffsview.h
        include/channel.h
        include/CompButtons.h
        include/bossesview.h
        include/bosspanel.h
        include/statsview.h
)

set(FORMS
        forms/mainwindow.ui
        forms/gamedisplay.ui
        forms/heroesview.ui
        forms/hostpage.ui
        forms/heropanel.ui
        forms/stuffpanel.ui
        forms/stuffsview.ui
        forms/channel.ui
        forms/bossesview.ui
        forms/bosspanel.ui
        forms/statsview.ui
    )

add_library(view STATIC ${SOURCES} ${HEADERS} ${FORMS}
     )
target_include_directories(view PUBLIC include forms)

target_link_libraries(view
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    core
)
