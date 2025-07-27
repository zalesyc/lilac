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
    find_package(Qt6 COMPONENTS DBus)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(LilacStyle SHARED
    src/lilac.json
    src/style_plugin.cpp
    src/style_plugin.h
    src/style.cpp
    src/style.h
    src/utils.cpp
    src/utils.h
    src/animation_manager.cpp
    src/animation_manager.h
)

target_link_libraries(LilacStyle PRIVATE Qt6::Widgets)

if (NOT NO_SETTINGS)
    add_library(LilacSettings SHARED
        src/settings/settings_app.cpp
        src/settings/settings_app.h
        src/settings/settings_app.ui
    )
    kconfig_add_kcfg_files(LilacSettings settings.kcfgc)
    target_link_libraries(LilacSettings PRIVATE Qt6::Widgets)
    target_link_libraries(LilacSettings PRIVATE KF6::ConfigCore KF6::ConfigGui)


    add_executable(LilacSettingsApp
        src/settings/settings_app.cpp
        src/settings/settings_app.h
        src/settings/settings_app.ui
        src/settings/main.cpp
    )
    kconfig_add_kcfg_files(LilacSettingsApp settings.kcfgc)
    target_link_libraries(LilacSettingsApp PRIVATE Qt6::Widgets)
    target_link_libraries(LilacSettingsApp PRIVATE KF6::ConfigCore KF6::ConfigGui)

    if (Qt6DBus_FOUND)
        target_link_libraries(LilacSettings PRIVATE Qt6::DBus)
        target_compile_definitions(LilacSettings PRIVATE HAS_DBUS=1)
        target_link_libraries(LilacSettingsApp PRIVATE Qt6::DBus)
        target_compile_definitions(LilacSettingsApp PRIVATE HAS_DBUS=1)
    else()
        target_compile_definitions(LilacSettings PRIVATE HAS_DBUS=0)
        target_compile_definitions(LilacSettingsApp PRIVATE HAS_DBUS=0)
    endif()
else()
    target_compile_definitions(LilacSettings PRIVATE HAS_DBUS=0)
    target_compile_definitions(LilacSettingsApp PRIVATE HAS_DBUS=0)
endif()

if (NOT NO_KSTYLE)
    target_link_libraries(LilacStyle PRIVATE KF6::Style)
    target_compile_definitions(LilacStyle PRIVATE HAS_KSTYLE=1)
else()
    target_compile_definitions(LilacStyle PRIVATE HAS_KSTYLE=0)
endif()

if (NOT NO_KCOLORSCHEME)
    target_link_libraries(LilacStyle PRIVATE KF6::ColorScheme)
    target_compile_definitions(LilacStyle PRIVATE HAS_KCOLORSCHEME=1)
else()
    target_compile_definitions(LilacStyle PRIVATE HAS_KCOLORSCHEME=0)
endif()

if (NOT NO_SETTINGS)
    kconfig_add_kcfg_files(LilacStyle settings.kcfgc)
    target_link_libraries(LilacStyle PRIVATE KF6::ConfigCore KF6::ConfigGui)
    target_compile_definitions(LilacStyle PRIVATE HAS_SETTINGS=1)

    if (Qt6DBus_FOUND)
        target_link_libraries(LilacStyle PRIVATE Qt6::DBus)
        target_compile_definitions(LilacStyle PRIVATE HAS_DBUS=1)
    else()
        target_compile_definitions(LilacStyle PRIVATE HAS_DBUS=0)
    endif()
else()
    target_compile_definitions(LilacStyle PRIVATE HAS_SETTINGS=0)
    target_compile_definitions(LilacStyle PRIVATE HAS_DBUS=0)
endif()

target_compile_definitions(LilacStyle PRIVATE LILAC_LIBRARY)
set_target_properties(LilacStyle PROPERTIES
    OUTPUT_NAME "Lilac"
)

install(
    TARGETS LilacStyle
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/styles
)

if (NOT NO_SETTINGS)
    set_target_properties(LilacSettings PROPERTIES
        OUTPUT_NAME "LilacSettings"
    )
    install(
        TARGETS LilacSettings
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/kstyle_config
    )
    install(
        FILES src/settings/lilac.themerc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kstyle/themes
    )
    set_target_properties(LilacSettingsApp PROPERTIES
        OUTPUT_NAME "lilacsettings"
    )
    install(
        TARGETS LilacSettingsApp
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

