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
     */
    outline, // outline or separator line
    button,  // DHP
    toggleButtonChecked,
    checkBoxInside,  // D all checkbox colors are also for radiobuttons
    checkBoxCheck,   // D
    checkBoxOutline, // D
    tabCheckedOutline,
    tabCheckedFill,
    tabUncheckedHover,
    tabWidgetPageArea,
    scrollBarHoverBackground,
    scrollBarHoverOutline,
    scrollBarSlider,         // HP
    sliderHandle,            // D
    sliderHandleHoverCircle, // P
    sliderLineBefore,        // D
    sliderLineAfter,         // D
    sliderTickmarks,
    lineEditBackground,          // D
    lineEditOutline,             // DF
    spinBoxBackground,           // D
    spinBoxOutline,              // DF
    spinBoxIndicator,            // DHP
    spinBoxIndicatorHoverCircle, // HP
    focusColor,
    other,
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
constexpr int btnRadius = 12;
constexpr int sliderHandleHoverCircleDiameter = 32;
constexpr int sliderHandleCircleDiameter = 12;
constexpr int sliderTickmarksLen = 8;
constexpr int spinBoxSpacing = btnRadius / 2;
constexpr int spinBoxIndicatorWidth = 30;
constexpr int spinIndicatorSize = 10;
constexpr int spinIndicatorHoverCircleSize = 24;
} // namespace Constants

} // namespace Orchid
