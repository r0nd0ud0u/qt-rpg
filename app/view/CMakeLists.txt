project(view)

# Qt pre-compilation activation
set(CMAKE_AUTOUIC_SEARCH_PATHS ./forms)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
        src/mainwindow.cpp
        src/character.cpp
        src/gamedisplay.cpp
        src/heroesview.cpp
        src/hostpage.cpp
        src/heropanel.cpp
)

set(HEADERS
        include/mainwindow.h
        include/character.h
        include/gamedisplay.h
        include/heroesview.h
        include/hostpage.h
        include/heropanel.h
)

set(FORMS
        forms/mainwindow.ui
        forms/gamedisplay.ui
        forms/heroesview.ui
        forms/hostpage.ui
        forms/heropanel.ui
)

add_library(view STATIC ${SOURCES} ${HEADERS} ${FORMS})

target_include_directories(view PUBLIC include forms)

target_link_libraries(view
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)
