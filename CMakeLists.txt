cmake_minimum_required(VERSION 3.14)
project(Lilac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(REQUIRE_KSTYLE "Whether to set kstyle as a required dependency" OFF)
option(NO_KSTYLE "Whether to not use kstyle" OFF)

option(REQUIRE_KCOLORSCHEME "Whether to set KColorScheme as a required dependency" OFF)
option(NO_KCOLORSCHEME "Whether to not use kstyle" OFF)

find_package(QT NAMES Qt6 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(REQUIRE_KSTYLE)
    find_package(KF6FrameworkIntegration "6.5.0...<7.0.0" CONFIG REQUIRED)
elseif(NOT NO_KSTYLE)
    find_package(KF6FrameworkIntegration "6.5.0...<7.0.0" CONFIG)
endif()

if(REQUIRE_KCOLORSCHEME)
    find_package(KF6ColorScheme "6.5.0...<7.0.0" CONFIG REQUIRED)
elseif(NOT NO_KCOLORSCHEME)
    find_package(KF6ColorScheme "6.5.0...<7.0.0" CONFIG)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(Lilac SHARED
    lilacstyleplugin.cpp
    lilacstyleplugin.h
    lilacstyle.cpp
    lilac.json
    lilacstyle.cpp
    lilacstyle.h
    lilac.cpp
    lilac.h
)

target_link_libraries(Lilac PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if (KF6FrameworkIntegration_FOUND AND (NOT NO_KSTYLE))
    target_link_libraries(Lilac PRIVATE KF6::Style)
    target_compile_definitions(Lilac PUBLIC HAS_KSTYLE=1)
else()
    target_compile_definitions(Lilac PUBLIC HAS_KSTYLE=0)
endif()

if (KF6ColorScheme_FOUND AND (NOT NO_KCOLORSCHEME))
    target_link_libraries(Lilac PRIVATE KF6::ColorScheme)
    target_compile_definitions(Lilac PUBLIC HAS_KCOLORSCHEME=1)
else()
    target_compile_definitions(Lilac PUBLIC HAS_KCOLORSCHEME=0)
endif()

target_compile_definitions(Lilac PRIVATE LILAC_LIBRARY)

install(TARGETS
    Lilac
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/plugins/styles

)

