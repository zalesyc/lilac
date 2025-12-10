// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QBrush>
#include <QColor>
#include <QPalette>
#include <QPen>

#include "utils/state.h"

namespace Lilac {

enum Color {
    /*
     * letters after elements are for their possible states, that should be handled
     * D: disabled, H: hover, P: pressed, F: has focus
     *
     * shortcuts:
     * Bg - background
     * Btn - button
     */
    line,  // outline or separator line

    focusRect,

    indicatorArrow,  // D

    buttonBg,             // DHP
    toggleButtonChecked,  // D

    checkBoxInside,              // D all checkbox colors are also for radiobuttons
    checkBoxCheck,               // D
    checkBoxOutline,             // D
    checkBoxHoverCircle,         // HP
    checkBoxHoverCircleChecked,  // HP
    checkBoxText,                // D

    tabText,
    tabCheckedOutline,
    tabCheckedFill,
    tabUncheckedHover,
    tabWidgetPageArea,
    tabCloseIndicator,             // DHP, just the x
    tabCloseIndicatorHoverCircle,  // HP

    scrollBarHoverBg,
    scrollBarHoverOutline,
    scrollBarSlider,  // HPD

    sliderHandle,             // D
    sliderHandleHoverCircle,  // HPF - hover is the default
    sliderLineBefore,         // D, the colored part
    sliderLineAfter,          // D, the gray part
    sliderTickmarks,

    lineEditBg,       // D
    lineEditOutline,  // DFH

    spinBoxBg,                    // D
    spinBoxOutline,               // DFH
    spinBoxIndicator,             // DHP
    spinBoxIndicatorHoverCircle,  // HP

    comboBoxBg,                // DHPF - pressed: menu is open
    comboBoxOutline,           // DHPF
    comboBoxFramelessOutline,  // DHPF
    comboBoxUneditableText,    // D
    comboBoxPopupBg,
    comboBoxPopupShadow,

    menuText,          // D
    menuShortcutText,  // D
    menuSeparator,     // D
    menuOutline,
    menuItemHoverBg,
    menuBg,
    menuShadow,
    menuHighlight,       // small line on the top of the menu to improve contrast, only for dark mode
    menuBarItemHoverBg,  // HP - hover: hover without open menu, pressed: menu open
    menuBarItemText,     // D
    menuBarBg,           // D

    toolBtnText,               // D
    toolBtnBg,                 // DHP
    toolBtnBgAutoRise,         // DHP
    toolBtnBgChecked,          // D
    toolBtnBgAutoRiseChecked,  // DHP
    toolBtnMenuSeparator,      // D
    toolBtnFocusOutline,

    toolBarBgOther,    // D - all positions except for top
    toolBarBgHeader,   // D - only if on top
    toolBarHandle,     // D
    toolBarSeparator,  // D

    progressBarIndicatorBg,  // D
    progressBarIndicator,    // D
    progressBarText,         // D

    branchIndicator,

    groupBoxLine,  // D
    groupBoxText,  // D

    viewHeaderBg,           // DHP - when with table: pressed - the row/collumn is selected
    viewHeaderEmptyAreaBg,  // D

    dialLineBefore,         // D
    dialLineAfter,          // D
    dialHandle,             // D
    dialHandleHoverCircle,  // HPF - hover is the default

    rubberbandLine,  // when shape is line
    rubberBandRectOutline,
    rubberBandRectBg,
    rubberBandRectBgOpaque,

    dockWidgetTitle,  // D
    dockWidgetFloatingBg,

    itemViewItemBg,                  // DHP - drawn over the color provided by the widget, may be just a semi-transparent overlay; pressed: item is selected
    itemViewItemDefaultAlternateBg,  // itemView background, where the widget says that it wants alternateBase
    itemViewText,                    // DP - pressed: the item is selected

    tooltipBg,

};

const QColor getColor(const QPalette& pal, const Color color, const State& state = State());
const QBrush getBrush(const QPalette& pal, const Color color, const State& state = State());
const QPen getPen(const QPalette& pal, const Color color, const State& state, const qreal penWidth = 1);  // this needs to have an overload due to
const QPen getPen(const QPalette& pal, const Color color, const qreal penWidth = 1);                      // the way optional parameters are handled

const bool isDarkMode(const QPalette& pal);

}  // namespace Lilac
