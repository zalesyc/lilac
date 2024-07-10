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
    outline,
    buttonBackground,
    disabledButtonBackground,
    buttonHover,
    buttonClicked,
    buttonChecked,  // tooggle button
    checkBoxInside, // all checkbox colors are also for radiobuttons
    checkBoxCheck,
    checkBoxOutline,
    disabledCheckBoxInside,
    disabledCheckBoxCheck,
    disabledCheckBoxOutline,
    tabCheckedOutline,
    tabCheckedFill,
    tabUncheckedHover,
    tabWidgetPageArea,
    scrollBarHoverBackground,
    scrollBarHoverOutline,
    scrollBarSlider,
    scrollbarSliderHover,
    scrollbarSliderClick,
    focusColor,
    other,
};

QColor getColor(const QPalette& pal, const Color color);
QBrush getBrush(const QPalette& pal, const Color color);
bool isDarkMode(const QPalette& pal);

namespace Constants {
constexpr int btnRadius = 10;
}

} // namespace Orchid
