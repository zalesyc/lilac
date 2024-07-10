cmake_minimum_required(VERSION 3.14)
project(Orchid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(Orchid SHARED
    orchidstyleplugin.cpp
    orchidstyleplugin.h
    orchidstyle.cpp
    orchid.json
    orchidstyle.cpp
    orchidstyle.h
    orchid.cpp
    orchid.h
)

target_link_libraries(Orchid PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(Orchid PRIVATE ORCHID_LIBRARY)


install(TARGETS
    Orchid
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/plugins/styles

)

