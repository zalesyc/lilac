# Lilac

**Lilac**, a Qt6 theme inspired by the [Orchis GTK theme](https://github.com/vinceliuice/Orchis-theme) by [Vinceliuice](https://github.com/vinceliuice)

## About
A native Qt6 application style for KDE Plasma that provides modern widget styling without relying on Kvantum. This theme fully supports KDE color schemes, automatically adapting to your system colors.

This is strictly an application style - it handles only the appearance of Qt widgets and interface elements. It does not include icons, color schemes, or window decorations. Users are encouraged to pair it with their preferred color scheme, icon theme, window decorations…


![A screenshot showing Qt gallery using lilac-qt](./screenshot.png)
Screenshot: Icons: Fluent, ColorScheme: Breeze-Light/FluentDark

## Building:
### Build Configuration
#### KStyle Integration

[KStyle](https://api.kde.org/frameworks/frameworkintegration/html/classKStyle.html) provides enhanced integration with KDE Plasma. If you dont use KStyle, QCommonStyle is used. You can configure its usage during build time:

- **Default behavior**: `OFF` (i.e. KStyle will be used)
- **Available options**:  
  - `-DNO_KSTYLE=ON`: Doesn't use KStyle

**Recommendation**: Use KStyle with KDE Plasma, otherwise probably use QCommonStyle (KStyle has a lot of dependencies)

#### KColorScheme Support

[KColorScheme](https://api.kde.org/frameworks/kcolorscheme/html/classKColorScheme.html) extends QPalette functionality to provide better color consistency across KDE applications.

- **Default behavior**: `OFF` (i.e. KColorScheme will be used)
- **Available options**:  
    `-DNO_KCOLORSCHEME=ON`: Disables KColorScheme support

**Note**: KColorScheme is recommended for better color consistency, but can be disabled to reduce dependencies.

#### Settings

Whether to enable settings, this requires `QDbus` and `KConfig` as dependencies. 
When enabled, two aditional targets will be created, a standalone settings app and a library to integrate with the KDE plasma systemsetings.

- **Default behavior**: `OFF` (i.e. Settings will be enabled)
- **Available options**:  
    `-DNO_SETTINGS=ON`: Disables settings

### Installation steps:

**Note:** this theme support **only Qt >= 6.6.0**

1. **Install dependencies**

    `TODO`  

2. **Clone the repository** 

    ~~~ shell
    git clone https://github.com/zalesyc/lilac-qt.git && cd lilac-qt
    ~~~

3. **Configure**

    3.1 create the build directory
    ~~~ shell
    mkdir build && cd build
    ~~~

    3.2 Configure the project with cmake

    here you can add the options from the Build Configuration section

    ~~~ shell
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
    ~~~

4. **Build and Install**

    4.1 Compile
    ~~~ shell
    cmake --build . --parallel 
    ~~~

    4.2 Install
    ~~~ shell
    sudo cmake --install . 
    ~~~

The style can now be selected from system settings

## Issues / TODO:

- Right-to-left language support  
- Implement animations  
- Allow for translucent menus, and maybe also translucent windows  
  
