# Orchid


## Docs
- https://doc.qt.io/qt-6/style-reference.html
- https://doc.qt.io/qt-6/qstyle.html


## Implemented Widgets
Taken from the [docs](https://doc.qt.io/qt-6/style-reference.html#widget-reference)
- [x] Push Buttons 
- [x] Check and Radio Buttons
- [x] Tabs
- [ ] Scroll Bars - **Partially**
- [x] Sliders
- [ ] Spin Boxes
- [ ] Title Bar
- [ ] Combo Box
- [ ] Group Boxes
- [ ] Splitters
- [ ] Progress Bar
- [ ] Tool Buttons
- [ ] Toolbars
- [ ] Menus
- [ ] Menu Bar
- [ ] Item View Headers
- [ ] Tree Branch Indicators
- [ ] Tool Boxes
- [ ] Size Grip
- [ ] Rubber Band
- [ ] Dock Widgets

Widgets not mentioned in the docs
- [ ] Dial

## TODO
- `Style::drawComplexControl` -> CC_Slider `std::acos` is expensive so remove it - the parameters are constants from PixelMetric so it should be replaced by a precalculated value
