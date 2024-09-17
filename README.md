# Orchid


## Docs and useful
- https://doc.qt.io/qt-6/style-reference.html
- https://doc.qt.io/qt-6/qstyle.html
- https://www.olivierclero.com/code/custom-qstyle/


## Implemented Widgets
Taken from the [docs](https://doc.qt.io/qt-6/style-reference.html#widget-reference)
- [x] Push Buttons 
- [x] Check and Radio Buttons
- [x] Tabs
- [ ] Scroll Bars - **Partially**
- [x] Sliders
- [x] Spin Boxes
- [ ] Title Bar
- [ ] Combo Box
- [ ] Group Boxes
- [ ] Splitters
- [ ] Progress Bar
- [ ] Tool Buttons
- [ ] Toolbars
- [x] Menus
- [x] Menu Bar
- [ ] Item View Headers
- [ ] Tree Branch Indicators
- [ ] Tool Boxes
- [ ] Size Grip
- [ ] Rubber Band
- [ ] Dock Widgets

Widgets not mentioned in the docs
- [ ] Dial
- [x] LineEdits 

## TODO
- `Style::drawComplexControl` -> CC_Slider `std::acos` is expensive so remove it - the parameters are constants from PixelMetric so it should be replaced by a precalculated value
- look at `PE_IndicatorArrow*` -> there is commented out implementation that has some bugs
