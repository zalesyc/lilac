# Lilac


## Docs
- https://doc.qt.io/qt-6/style-reference.html
- https://doc.qt.io/qt-6/qstyle.html



## Implemented Widgets
Taken from the [docs](https://doc.qt.io/qt-6/style-reference.html#widget-reference)
- [x] Push Buttons 
- [x] Check and Radio Buttons
- [x] Tabs
- [x] Scroll Bars
- [x] Sliders
- [x] Spin Boxes
- [ ] Title Bar
- [x] Combo Box - **Partially:** without the popup menu
- [x] Group Boxes
- [ ] Splitters
- [x] Progress Bar
- [x] Tool Buttons
- [x] Toolbars
- [x] Menus
- [x] Menu Bar
- [x] Item View Headers
- [x] Tree Branch Indicators
- [ ] Tool Boxes
- [x] Size Grip
- [x] Rubber Band
- [x] Dock Widgets

Widgets not mentioned in the docs
- [x] Dial
- [x] LineEdits 
- [x] Frames
- [x] Tooltips

## TODO
- `Style::drawComplexControl` -> CC_Slider `qAcos` is expensive so remove it - the parameters are constants from PixelMetric so it should be replaced by a precalculated value
- `CE_ProgressBarContents` -> make the busy indicator animated

## Compilation options
### Kstyle:
when you build lilac, you can select whether to inherit from KStyle, which is recomended when you use kde plasma
or QCommonStyle, which is recommended othervise. 

By default, kstyle is used when it is already installed, othervise QCommonStyle.

There are two cmake flags to change this behavior:
1. `-DREQUIRE_KSTYLE=ON` to set kstyle as required dependency 
2. `-DNO_KSTYLE=ON` to not use kstyle


## Useful resources
- https://www.olivierclero.com/code/custom-qstyle/ 
- kvantumpreview, for testing widgets: https://github.com/tsujan/Kvantum/tree/master/Kvantum/kvantumpreview
- Qt galllery, also for widget testing (not as comprehensive as kvantumpreview): https://doc.qt.io/qt-6/gallery.html
- gammaray, useful for finding out what type of widget a widget is in a app: https://www.kdab.com/development-resources/qt-tools/gammaray/
- Qt source code: https://github.com/qt/qtbase

## Clang-format config
    ---
    TabWidth: 4
    IndentWidth: 4
    PointerAlignment: Left
    ColumnLimit: 0
    BreakBeforeTernaryOperators: false
    IndentCaseLabels: true
    ...
