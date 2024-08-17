#pragma once

#include <QCommonStyle>
#include <QPainter>
#include <QPair>
#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

namespace Orchid {
using SuperStyle = QCommonStyle;

enum Color {
    /*
     * letters after elements are for their possible states, that should be handled
     * D: disabled, H: hover, P: pressed
     */
    outline,
    button, // DHP
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
    focusColor,
    other,
};

struct State {
    State() {}
    State(const QStyle::State& state);

    bool enabled = true;  // QStyle::State_Enabled
    bool hovered = false; // QStyle::State_MouseOver
    bool pressed = false; // QStyle::State_Sunken
};

QColor getColor(const QPalette& pal, const Color color, const State& state = State());
QBrush getBrush(const QPalette& pal, const Color color, const State& state = State());
bool isDarkMode(const QPalette& pal);

namespace Constants {
constexpr int btnRadius = 10;
constexpr int sliderHandleHoverCircleDiameter = 32;
constexpr int sliderHandleCircleDiameter = 12;
constexpr int sliderTickmarksLen = 8;
} // namespace Constants

} // namespace Orchid
