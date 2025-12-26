// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#include "colors.h"
#include "config.h"

#if HAS_KCOLORSCHEME
#include <KColorScheme>
#endif

namespace Lilac {

using CRole = QPalette::ColorRole;
using CGroup = QPalette::ColorGroup;

#if HAS_KCOLORSCHEME
using KCSet = KColorScheme::ColorSet;
using KBgRole = KColorScheme::BackgroundRole;
using KDcRole = KColorScheme::DecorationRole;
using KFgRole = KColorScheme::ForegroundRole;
using KShRole = KColorScheme::ShadeRole;
#endif

static QColor getColorFromPallete(const QPalette& pal, const Color color, const State& state);
static QColor getColorFromKColorScheme(const QPalette& pal, const Color color, const State& state);
QColor lessContrastingBg(const QPalette& pal, const CGroup cgroup);
CGroup groupFromState(const State& state);
QColor mixAtContrast(const QColor& background, const QColor& foreground, const qreal contrast);

const QColor getColor(const QPalette& pal, const Color color, const State& state) {
#if HAS_KCOLORSCHEME
    return getColorFromKColorScheme(pal, color, state);
#else
    return getColorFromPallete(pal, color, state);
#endif
}

const QBrush getBrush(const QPalette& pal, const Color color, const State& state) {
    return QBrush(getColor(pal, color, state));
}

const QPen getPen(const QPalette& pal, const Color color, const State& state, const qreal penWidth) {
    return QPen(QBrush(getColor(pal, color, state)), penWidth);
}
const QPen getPen(const QPalette& pal, const Color color, const qreal penWidth) {
    return QPen(QBrush(getColor(pal, color)), penWidth);
}

static QColor getColorFromPallete(const QPalette& pal, const Color color, const State& state) {
    switch (color) {
        case groupBoxLine:
        case line: {
            return mixAtContrast(pal.color(CGroup::Normal, CRole::Window),
                                 pal.color(CGroup::Normal, CRole::Text),
                                 0.1);
        }

        case focusRect:
            return pal.color(CGroup::Normal, CRole::Highlight);

        case toolBtnBg:
        case buttonBg: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Button);

            const auto base = pal.color(CGroup::Normal, CRole::Button);
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
            return base;
        }

        case toolBtnBgChecked:
        case toggleButtonChecked:
            return pal.color(groupFromState(state), CRole::Accent);

        case checkBoxCheck:
            return pal.color(CGroup::Normal, CRole::Window);

        case checkBoxOutline: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            const auto base = pal.color(CGroup::Normal, CRole::Text);
            if (state.hasFocus)
                return base;
            return isDarkMode(pal) ? base.darker(120) : base.lighter(120);
        }
        case checkBoxInside: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            return pal.color(CGroup::Normal, CRole::Accent);
        }
        case checkBoxHoverCircleChecked: {
            QColor base = getColor(pal, Color::checkBoxInside, state);
            if (state.pressed) {
                base.setAlpha(40);
                return base;
            }
            base.setAlpha(20);
            return base;
        }
        case checkBoxHoverCircle: {
            QColor base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed) {
                base.setAlpha(40);
                return base;
            }
            base.setAlpha(20);
            return base;
        }

        case tabCheckedOutline: {
            const auto base = getColor(pal, Color::line);
            if (state.hasFocus) {
                return pal.color(CGroup::Normal, CRole::Accent);
            }
            return base;
        }

        case tabCheckedFill:
        case tabUncheckedHover:
        case tabWidgetPageArea:
            return pal.color(CGroup::Normal, CRole::Base);

        case scrollBarHoverBg: {
            const auto base = pal.color(CGroup::Normal, CRole::Base);
            return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
        }

        case scrollBarHoverOutline:
            return getColor(pal, Color::line);

        case scrollBarSlider: {
            if (!state.enabled)
                return getColor(pal, Color::scrollBarHoverBg);

            auto base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed)
                base.setAlpha(120);
            else if (state.hovered)
                base.setAlpha(100);
            else
                base.setAlpha(80);
            return base;
        }

        case sliderHandleHoverCircle:
        case dialHandleHoverCircle: {
            auto base = pal.color(CGroup::Normal, CRole::Accent);
            if (state.pressed)
                base.setAlpha(100);
            else if (state.hasFocus && !state.hovered)
                base.setAlpha(30);
            else
                base.setAlpha(70);
            return base;
        }

        case sliderHandle:
        case dialHandle:
        case sliderLineBefore:
        case dialLineBefore:
            if (!state.enabled) {
                const auto base = lessContrastingBg(pal, CGroup::Normal);
                return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
            }
            return pal.color(CGroup::Normal, CRole::Accent);

        case sliderTickmarks:
        case sliderLineAfter:
        case dialLineAfter: {
            const auto base = lessContrastingBg(pal, CGroup::Normal);
            if (!state.enabled)
                return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
            return isDarkMode(pal) ? base.lighter(160) : base.darker(185);
        }

        case lineEditBg:
        case spinBoxBg:
        case comboBoxBg:
            return pal.color(groupFromState(state), CRole::Button);

        case lineEditOutline:
        case spinBoxOutline: {
            if (!state.enabled)
                return getColor(pal, lineEditBg, state);
            if (state.hasFocus)
                return pal.color(CGroup::Normal, CRole::Accent);
            const QColor base = getColor(pal, lineEditBg, state);

            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);

            return base;
        }

        case spinBoxIndicator:
        case tabCloseIndicator: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            const QColor base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(150) : base.darker(150);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
            return base;
        }
        case spinBoxIndicatorHoverCircle:
        case tabCloseIndicatorHoverCircle: {
            QColor base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed) {
                base.setAlpha(100);
                return base;
            }
            if (state.hovered) {
                base.setAlpha(40);
                return base;
            }
            return base;
        }

        case comboBoxOutline: {
            if (!state.enabled)
                return getColor(pal, lineEditBg, state);
            if (state.pressed || state.hasFocus)
                return pal.color(CGroup::Normal, CRole::Accent);

            const QColor base = getColor(pal, lineEditBg, state);

            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);

            return base;
        }

        case comboBoxFramelessOutline: {
            if (state.enabled && (state.hovered || state.hasFocus || state.pressed)) {
                return getColor(pal, comboBoxOutline, state);
            }
            return QColor(Qt::transparent);
        }

        case menuText:
        case menuBarItemText:
        case comboBoxUneditableText:
        case toolBtnText:
        case progressBarText:
        case groupBoxText:
        case tabText:
        case checkBoxText:
        case dockWidgetTitle:
        case itemViewText:
            return pal.color(groupFromState(state), CRole::Text);

        case menuShortcutText:
            if (isDarkMode(pal))
                return getColor(pal, Color::menuText, state).darker(130);
            return getColor(pal, Color::menuText, state).lighter(130);

        case comboBoxPopupBg:
            return pal.color(CGroup::Normal, CRole::AlternateBase);

        case menuBg: {
            QColor base = pal.color(CGroup::Normal, CRole::AlternateBase);
            base.setAlpha(Config::get().menuBgOpacity);
            return base;
        }

        case menuItemHoverBg:
        case menuBarItemHoverBg:
            return isDarkMode(pal) ? QColor(255, 255, 255, 26) : QColor(0, 0, 0, 26);

        case menuSeparator: {
            QColor base = getColor(pal, Color::line, state);
            base.setAlpha(qMax<int>(140, 255 - Config::get().menuBgOpacity));
            return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
        }

        case menuOutline: {
            QColor mixed = mixAtContrast(getColor(pal, Color::menuBg, state),
                                         getColor(pal, Color::line, state),
                                         qMax<qreal>(.5, (255 - Config::get().menuBgOpacity) / 255.0));
            mixed.setAlpha(qMax<int>(Config::get().menuBgOpacity, 140));
            return mixed;
        }

        case toolBarBgHeader:
        case toolBarBgOther:
        case viewHeaderEmptyAreaBg:
        case menuBarBg:
            return pal.color(groupFromState(state), CRole::Base);

        case comboBoxPopupShadow:
        case menuShadow:
            return isDarkMode(pal) ? QColor(0, 0, 0, 130) : QColor(0, 0, 0, 100);

        case toolBarHandle:
        case toolBarSeparator: {
            const QColor base = getColor(pal, Color::line, state);
            return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
        }

        case menuHighlight:
            return QColor(255, 255, 255, 35);

        case toolBtnBgAutoRise:
            if (!state.enabled)
                return QColor(Qt::transparent);

            if (state.pressed)
                return isDarkMode(pal) ? QColor(255, 255, 255, 40) : QColor(0, 0, 0, 40);
            if (state.hovered)
                return isDarkMode(pal) ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 30);
            return QColor(Qt::transparent);

        case toolBtnBgAutoRiseChecked: {
            if (!state.enabled) {
                QColor base = getColor(pal, Color::toolBtnBg, state);
                base.setAlpha(120);
                return base;
            }
            if (state.pressed)
                return isDarkMode(pal) ? QColor(255, 255, 255, 45) : QColor(0, 0, 0, 45);
            if (state.hovered)
                return isDarkMode(pal) ? QColor(255, 255, 255, 38) : QColor(0, 0, 0, 38);
            return isDarkMode(pal) ? QColor(255, 255, 255, 26) : QColor(0, 0, 0, 26);
        }

        case toolBtnMenuSeparator:
            if (!state.enabled)
                return isDarkMode(pal) ? pal.color(CGroup::Disabled, CRole::Button).lighter(130) :
                                         pal.color(CGroup::Disabled, CRole::Button).darker(130);
            return isDarkMode(pal) ? pal.color(CGroup::Normal, CRole::Button).lighter(200) :
                                     pal.color(CGroup::Normal, CRole::Button).darker(200);

        case toolBtnFocusOutline:
            return pal.color(CGroup::Normal, CRole::Accent);

        case indicatorArrow:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            return isDarkMode(pal) ? pal.color(CGroup::Normal, CRole::Text).darker(140) : pal.color(CGroup::Normal, CRole::Text).lighter(140);

        case progressBarIndicator:
            return pal.color(groupFromState(state), CRole::Accent);

        case progressBarIndicatorBg:
            return pal.color(groupFromState(state), CRole::Button);

        case viewHeaderBg:
            if ((state.pressed || state.hovered) && state.enabled)
                return getColor(pal, Color::buttonBg, state);
            return getColor(pal, Color::viewHeaderEmptyAreaBg, state);

        case branchIndicator: {
            const auto base = getColor(pal, Color::line, state);
            return isDarkMode(pal) ? base.lighter(125) : base.darker(105);
        }

        case rubberbandLine:
        case rubberBandRectOutline:
        case rubberBandRectBgOpaque:
            return pal.color(CGroup::Normal, CRole::Accent);

        case rubberBandRectBg: {
            auto base = getColor(pal, rubberbandLine);
            base.setAlpha(60);
            return base;
        }

        case dockWidgetFloatingBg:
            return pal.color(CGroup::Inactive, CRole::Window);

        case itemViewItemBg:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Base);
            if (state.pressed) {
                auto base = pal.color(CGroup::Normal, CRole::Accent);
                base.setAlpha(200);
                return base;
            }
            if (state.hovered)
                return isDarkMode(pal) ? QColor(255, 255, 255, 15) : QColor(0, 0, 0, 15);
            return QColor(Qt::transparent);

        case itemViewItemDefaultAlternateBg:
            return pal.color(CGroup::Normal, CRole::AlternateBase);

        case groupBoxAltStyleBg:
            return pal.color(CGroup::Normal, CRole::Base);

        case tooltipBg:
            return QColor(20, 20, 20, 235);

        default:
            break;
    }
    return QColor(Qt::red);
}

#if HAS_KCOLORSCHEME
static QColor getColorFromKColorScheme(const QPalette& pal, const Color color, const State& state) {
    switch (color) {
#if KCOLORSCHEME_FRAME_CONTRAST
        case groupBoxLine:
        case line: {
            return mixAtContrast(pal.color(CGroup::Normal, CRole::Window),
                                 pal.color(CGroup::Normal, CRole::Text),
                                 KColorScheme::frameContrast());
        }
#endif
        case toolBtnBg:
        case buttonBg: {
            const auto base = KColorScheme(groupFromState(state), KCSet::Button).background(KBgRole::NormalBackground).color();
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
            return base;
        }

        case groupBoxText:
        case menuText:
        case tabText:
        case checkBoxText:
        case dockWidgetTitle:
            return KColorScheme(groupFromState(state), KCSet::Window).foreground(KFgRole::NormalText).color();

        case menuBarItemText:
            return KColorScheme(groupFromState(state), KCSet::Header).foreground(KFgRole::NormalText).color();

        case comboBoxUneditableText:
        case toolBtnText:
        case progressBarText:
            return KColorScheme(groupFromState(state), KCSet::Button).foreground(KFgRole::NormalText).color();

        case menuShortcutText:
            return KColorScheme(groupFromState(state), KCSet::Window).foreground(KFgRole::InactiveText).color();

        case comboBoxPopupBg:
            return KColorScheme(CGroup::Normal, KCSet::Window).background(KBgRole::AlternateBackground).color();

        case menuBg: {
            QColor base = KColorScheme(CGroup::Normal, KCSet::Window).background(KBgRole::AlternateBackground).color();
            base.setAlpha(Config::get().menuBgOpacity);
            return base;
        }

        case toolBarBgHeader:
        case menuBarBg:
            return KColorScheme(groupFromState(state), KCSet::Header).background(KBgRole::NormalBackground).color();

        case toolBarBgOther:
            return KColorScheme(groupFromState(state), KCSet::Window).background(KBgRole::NormalBackground).color();

        case tooltipBg: {
            auto base = KColorScheme(CGroup::Normal, KCSet::Tooltip).background(KBgRole::NormalBackground).color();
            base.setAlpha(Config::tooltipOpacity);
            return base;
        }

        case tabUncheckedHover:
        case tabCheckedFill:
        case tabWidgetPageArea:
            return KColorScheme(CGroup::Normal, KCSet::View).background(KBgRole::NormalBackground).color();

        default:
            break;
    }
    return getColorFromPallete(pal, color, state);
}
#endif

const bool isDarkMode(const QPalette& pal) {
    return pal.color(QPalette::ColorRole::Window).lightness() < pal.color(QPalette::ColorRole::Text).lightness();
}

QColor lessContrastingBg(const QPalette& pal, const CGroup cgroup) {
    const auto window = pal.color(cgroup, CRole::Window);
    const auto base = pal.color(cgroup, CRole::Base);
    if ((window.lightness() > base.lightness()) == isDarkMode(pal)) {
        return window;
    }
    return base;
}

CGroup groupFromState(const State& state) {
    return state.enabled ? CGroup::Active : CGroup::Disabled;
}

QColor mixAtContrast(const QColor& background, const QColor& foreground, const qreal contrast) {
    const qreal bgContrast = 1 - contrast;
    return QColor(qBound(0, qRound(background.red() * bgContrast + foreground.red() * contrast), 255),
                  qBound(0, qRound(background.green() * bgContrast + foreground.green() * contrast), 255),
                  qBound(0, qRound(background.blue() * bgContrast + foreground.blue() * contrast), 255),
                  qBound(0, qRound(background.alpha() * bgContrast + foreground.alphaF() * contrast), 255));
}

}  // namespace Lilac
