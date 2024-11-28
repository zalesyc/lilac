cmake_minimum_required(VERSION 3.14)
project(Orchid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(REQUIRE_KSTYLE "Whether to set kstyle as a required dependency" OFF)
option(NO_KSTYLE "Whether to not use kstyle" OFF)

find_package(QT NAMES Qt6 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (NO_KSTYLE)
    option(KF6FrameworkIntegration_FOUND OFF)
elseif(REQUIRE_KSTYLE)
    find_package(KF6FrameworkIntegration "6.5.0" CONFIG REQUIRED)
else()
    find_package(KF6FrameworkIntegration "6.5.0" CONFIG)
endif()

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

if (KF6FrameworkIntegration_FOUND)
    target_link_libraries(Orchid PRIVATE KF6::Style)
    target_compile_definitions(Orchid PUBLIC HAS_KSTYLE=1)
else()
    target_compile_definitions(Orchid PUBLIC HAS_KSTYLE=0)
endif()

target_compile_definitions(Orchid PRIVATE ORCHID_LIBRARY)

install(TARGETS
    Orchid
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/plugins/styles

)

