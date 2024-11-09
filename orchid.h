#pragma once

#include <QCommonStyle>
#include <QPainter>
#include <QPair>
#include <QPen>
#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

namespace Orchid {
using SuperStyle = QCommonStyle;

enum Color {
    /*
     * letters after elements are for their possible states, that should be handled
     * D: disabled, H: hover, P: pressed, F: has focus
     *
     * shortcuts:
     * Bg - background
     * Btn - button
     */
    line, // outline or separator line

    focusRect,

    indicatorArrow, // D

    buttonBg, // DHP
    toggleButtonChecked,

    checkBoxInside,             // D all checkbox colors are also for radiobuttons
    checkBoxCheck,              // D
    checkBoxOutline,            // D
    checkBoxHoverCircle,        // HP
    checkBoxHoverCircleChecked, // HP

    tabCheckedOutline,
    tabCheckedFill,
    tabUncheckedHover,
    tabWidgetPageArea,
    tabCloseIndicator,            // DHP, just the x
    tabCloseIndicatorHoverCircle, // HP

    scrollBarHoverBg,
    scrollBarHoverOutline,
    scrollBarSlider, // HPD

    sliderHandle,            // D
    sliderHandleHoverCircle, // P
    sliderLineBefore,        // D
    sliderLineAfter,         // D
    sliderTickmarks,

    lineEditBg,      // D
    lineEditOutline, // DFH

    spinBoxBg,                   // D
    spinBoxOutline,              // DFH
    spinBoxIndicator,            // DHP
    spinBoxIndicatorHoverCircle, // HP

    comboBoxBg,             // DHPF - pressed: menu is open
    comboBoxOutline,        // DHPF
    comboBoxUneditableText, // D

    menuText,         // D
    menuShortcutText, // D
    menuSeparator,    // D
    menuItemHoverBg,
    menuBg,
    menuShadow,
    menuBarItemHoverBg, // HP - hover: hover without open menu, pressed: menu open
    menuBarItemText,    // D
    menuBarBg,          // D

    toolBtnText,              // D
    toolBtnBg,                // DHP
    toolBtnBgAutoRise,        // DHP
    toolBtnBgChecked,         // DHP
    toolBtnBgAutoRiseChecked, // DHP
    toolBtnMenuSeparator,     // D
    toolBtnFocusOutline,

    toolBarBg,        // D
    toolBarHandle,    // D
    toolBarSeparator, // D

    progressBarIndicatorBg, // D
    progressBarIndicator,   // D
    progressBarText,        // D

    branchIndicator,

    groupBoxLine, // D
    groupBoxText, // D

    viewHeaderBg,          // DHP - when with table: pressed - the row/collumn is selected
    viewHeaderEmptyAreaBg, // D

    dialLineBefore,        // D
    dialLineAfter,         // D
    dialHandle,            // D
    dialHandleHoverCircle, // P

    rubberbandLine, // when shape is line
    rubberBandRectOutline,
    rubberBandRectBg,
    rubberBandRectBgOpaque,

};

struct State {
    State() {}
    State(const QStyle::State& state);

    bool enabled = true;   // QStyle::State_Enabled
    bool hovered = false;  // QStyle::State_MouseOver
    bool pressed = false;  // QStyle::State_Sunken
    bool hasFocus = false; // QStyle::State_HasFocus
};

const QColor getColor(const QPalette& pal, const Color color, const State& state = State());
const QBrush getBrush(const QPalette& pal, const Color color, const State& state = State());
const QPen getPen(const QPalette& pal, const Color color, const State& state, const qreal penWidth = 1); // this needs to have an overload due to
const QPen getPen(const QPalette& pal, const Color color, const qreal penWidth = 1);                     // the way optional parameters are handled

bool isDarkMode(const QPalette& pal);

namespace Constants {
constexpr int cornerRadius = 12;
constexpr int smallArrowSize = 10;

constexpr int controlsTextHorizontalPadding = 10; // Top and bottom; buttons, lineEdits, spinBoxes, comboboxes

constexpr int pushButtonMenuArrowPadding = 6; // length between the menu arrow and the right side

constexpr int checkBoxSize = 20;
constexpr int checkBoxHoverCircleSize = 34;

constexpr int tabCloseIndicatorSize = 6;         // size of the x, without the hover circle whose size is specified in
constexpr int tabBarStartPadding = 6;            // padding on the left/top side of the tabbar, if the tab alighnment is left, only for QTabWidget
constexpr int tabBarPaddingAboveTabs = 4;        // there is no rect for the tab position for both QTabBar and QTabWidget, the implementaion is hacky and would benefit from an improvement, e.g. reimplemntation of the relevant cases
constexpr int tabWidgetPageAreaCornerRadius = 6; // corner radius for the frame arount the contents of QTabWidget

constexpr int sliderHandleHoverCircleDiameter = 32;
constexpr int sliderHandleCircleDiameter = 12;
constexpr int sliderTickmarksLen = 8;

constexpr int scrollBarThickness = 18;
constexpr int scrollBarSliderPadding = 6; // left and right of the slider -> sets the thickness
constexpr int scrollBarSliderPaddingHover = 4;

constexpr int lineEditTextVerticalPadding = 6; // left and right
constexpr int lineEditMinWidthChars = 14;      // minimal width of lineedit: lineEditMinWidthChars * averageCharWidth

constexpr int spinIndicatorWidth = 30; // the width of the clickable space
constexpr int spinIndicatorIconSize = 10;
constexpr int spinIndicatorHoverCircleSize = 24;
constexpr int spinMinWidthChars = lineEditMinWidthChars;
constexpr int spinTextLeftPadding = lineEditTextVerticalPadding;

constexpr int menuHorizontalSpacing = 8;
constexpr int menuItemBorderRadius = 5;
constexpr int menuItemVerticalExternalPadding = 5;   // left and right of the menuItem, outside hover rect
constexpr int menuItemVerticalInternalPadding = 5;   // left and right, inside hover rect
constexpr int menuItemHorizontalInternalPadding = 5; // top and bottom, inside hover rect
constexpr int menuSeparatorHorizontalPadding = 3;    // top and bottom
constexpr int menuShadowSize = 6;                    // the size of the shadow,the shadow is added in polish.
constexpr int menuTransparentPadding = 7;            // transparent padding added to the menu, that contains the shadow and is spacing
constexpr int menuBorderRadius = 10;
constexpr int menuTransparency = 255; // value between 0 and 255, 0: transparent; 255: opaque
constexpr int menuSeparatorMinLen = 5;

constexpr int menuBarItemMinHeight = 24; // without the padding
constexpr int menuBarItemBorderRadius = 4;
constexpr int menuBarItemExternalPadding = 2;

constexpr int comboArrowWidth = 30; // the part that is clickable, the indicator is smaller
constexpr int comboMinWidthChars = lineEditMinWidthChars;
constexpr int comboTextLeftPadding = lineEditTextVerticalPadding; // used left of icon and left of text

constexpr int toolBtnMenuArrowSize = 8;                                      // the small arrow in the bottom left corner
constexpr QPoint toolBtnArrowOffset = QPoint(5, 5);                          // offset from bottomRight of the widget, for the small arrow in the bottom left
constexpr int toolBtnLabelHorizontalPadding = controlsTextHorizontalPadding; // Top and bottom
constexpr int toolBtnLabelVerticalPadding = 6;                               // left, right
constexpr int toolbtnLabelSpacing = 4;                                       // between the icon and text
constexpr int toolbtnArrowSectionWidth = 20;                                 // width of the separated arrow section on the right, poppupMode: MenuButtonPopup
constexpr int toolBtnMenuSeparatorHorizontalPadding = 4;

constexpr int toolBarHandleHorizontalPadding = 6;    // for the longer side (top and bottom, when toolbar is horizontal)
constexpr int toolBarSeparatorHorizontalPadding = 3; // for the longer side (top and bottom, when toolbar is horizontal)
constexpr int toolBarHandleVerticalPadding = 2;      // left and right, when toolbar is horizontal
constexpr int toolBarHandleLineSpacing = 2;

constexpr int progressBarLabelVerticalPadding = 3; // around the label, left and right when the progressbar is horizontal
constexpr int progressBarThickness = 6;

constexpr int treeIndicatorArrowSize = 10;
constexpr int treeIndicatorArrowLinePadding = 4; // the distance between the arrow and start of the line
constexpr int treeIndicatorLastCornerRadius = 5; // the radius of the curve on the last indicator

constexpr int groupBoxCheckSize = checkBoxSize;
constexpr int groupBoxTextCheckSpacing = 8;                             // spacing between the label and the checkbox
constexpr int groupBoxHeaderVerticalPadding = 5;                        // space between the end of the line and the label/check
constexpr int groupBoxLabelOffset = groupBoxHeaderVerticalPadding + 10; // length between the side of the groupbox and the label/check

constexpr int dialHandleDiameter = 12;
constexpr int dialHandleHoverCircleDiameter = 20;

constexpr int dialRangeNonWaraping = 300; // degrees
} // namespace Constants

} // namespace Orchid
