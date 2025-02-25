cmake_minimum_required(VERSION 3.14)
project(Lilac LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(NO_KSTYLE "Whether to not use KStyle" OFF)
option(NO_KCOLORSCHEME "Whether to not use KColorScheme" OFF)
option(NO_SETTINGS "Whether to not use KConfig and thus disabbling runtime settings" OFF)

find_package(QT NAMES Qt6 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(NOT NO_KSTYLE)
    find_package(KF6FrameworkIntegration "6.5.0...<7.0.0" CONFIG REQUIRED)
endif()

if(NOT NO_KCOLORSCHEME)
    find_package(KF6ColorScheme "6.5.0...<7.0.0" CONFIG REQUIRED)
endif()

if(NOT NO_SETTINGS)
    find_package(KF6Config "6.5.0...<7.0.0" CONFIG REQUIRED)
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

target_link_libraries(Lilac PRIVATE Qt6::Widgets)

if (NOT NO_KSTYLE)
    target_link_libraries(Lilac PRIVATE KF6::Style)
    target_compile_definitions(Lilac PRIVATE HAS_KSTYLE=1)
else()
    target_compile_definitions(Lilac PRIVATE HAS_KSTYLE=0)
endif()

if (NOT NO_KCOLORSCHEME)
    target_link_libraries(Lilac PRIVATE KF6::ColorScheme)
    target_compile_definitions(Lilac PRIVATE HAS_KCOLORSCHEME=1)
else()
    target_compile_definitions(Lilac PRIVATE HAS_KCOLORSCHEME=0)
endif()

if (NOT NO_SETTINGS)
    kconfig_add_kcfg_files(Lilac lilacsettings.kcfgc)
    target_link_libraries(Lilac PRIVATE KF6::ConfigCore KF6::ConfigGui)
    target_compile_definitions(Lilac PRIVATE HAS_SETTINGS=1)
else()
    target_compile_definitions(Lilac PRIVATE HAS_SETTINGS=0)
endif()

target_compile_definitions(Lilac PRIVATE LILAC_LIBRARY)

install(TARGETS
    Lilac
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/styles

)

