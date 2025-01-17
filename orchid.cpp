#include "orchid.h"

namespace Orchid {

using CRole = QPalette::ColorRole;
using CGroup = QPalette::ColorGroup;

static QColor getColorFromPallete(const QPalette& pal, const Color color, const State& state);
QColor lessContrastingBg(const QPalette& pal, const CGroup cgroup);

const QColor getColor(const QPalette& pal, const Color color, const State& state) {
    return getColorFromPallete(pal, color, state);
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
        case menuSeparator:
        case line: {
            const auto base = lessContrastingBg(pal, CGroup::Normal);
            return isDarkMode(pal) ? base.lighter(125) : base.darker(110);
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
            return pal.color(CGroup::Normal, CRole::Accent);

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
            return isDarkMode(pal) ? QColor(129, 201, 149) : QColor(14, 156, 87);
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

        case tabCheckedFill:
            return getColor(pal, Color::tabWidgetPageArea);

        case tabCheckedOutline: {
            const auto base = getColor(pal, Color::line);
            if (state.hasFocus) {
                return pal.color(CGroup::Normal, CRole::Accent);
            }
            return base;
        }

        case tabUncheckedHover:
            return getColor(pal, Color::tabCheckedFill);

        case tabWidgetPageArea:
            return pal.color(CGroup::Normal, CRole::Base);

        case scrollBarHoverBg: {
            const auto base = pal.color(CGroup::Normal, CRole::Base);
            return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
        }

        case scrollBarHoverOutline:
            return getColor(pal, Color::line);

        case scrollBarSlider: {
            const auto base = getColor(pal, Color::scrollBarHoverBg);
            if (!state.enabled)
                return base;
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(250) : base.darker(250);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(240) : base.darker(240);
            return isDarkMode(pal) ? base.lighter(230) : base.darker(130);
        }

        case sliderHandleHoverCircle:
        case dialHandleHoverCircle: {
            auto base = pal.color(CGroup::Normal, CRole::Accent);
            if (state.pressed) {
                base.setAlpha(80);
                return base;
            }
            base.setAlpha(50);
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

        case sliderLineAfter:
        case dialLineAfter: {
            const auto base = lessContrastingBg(pal, CGroup::Normal);
            if (!state.enabled)
                return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
            return isDarkMode(pal) ? base.lighter(160) : base.darker(185);
        }
        case sliderTickmarks:
            return getColor(pal, sliderLineAfter, state);

        case lineEditBg:
        case spinBoxBg:
        case comboBoxBg:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Button);
            return pal.color(CGroup::Normal, CRole::Button);

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

        case menuText:
        case menuBarItemText:
        case comboBoxUneditableText:
        case toolBtnText:
        case progressBarText:
        case groupBoxText:
        case tabText:
        case checkBoxText:
        case dockWidgetTitle:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            return pal.color(CGroup::Normal, CRole::Text);

        case menuShortcutText:
            if (isDarkMode(pal))
                return getColor(pal, Color::menuText, state).darker(130);
            return getColor(pal, Color::menuText, state).lighter(130);

        case menuBg: {
            QColor base = pal.color(CGroup::Normal, CRole::AlternateBase);
            base.setAlpha(Constants::menuBgOpacity);
            return base;
        }

        case menuItemHoverBg:
        case menuBarItemHoverBg:
            return isDarkMode(pal) ? QColor(255, 255, 255, 26) : QColor(0, 0, 0, 26);

        case toolBarBg:
        case viewHeaderEmptyAreaBg:
        case menuBarBg:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Base);
            return pal.color(CGroup::Normal, CRole::Base);

        case menuShadow: {
            QColor base = pal.color(CGroup::Normal, CRole::Shadow);
            return base;
        }
        case toolBarHandle:
        case toolBarSeparator: {
            const QColor base = getColor(pal, Color::line, state);
            return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
        }

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
                QColor base = pal.color(CGroup::Normal, CRole::Button);
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
            return pal.color(state.enabled ? CGroup::Normal : CGroup::Disabled, CRole::Accent);

        case progressBarIndicatorBg:
            return pal.color(state.enabled ? CGroup::Normal : CGroup::Disabled, CRole::Button);

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

        default:
            break;
    }
    return QColor(Qt::red);
}

State::State(const QStyle::State& state) {
    enabled = state & QStyle::State_Enabled;
    hovered = state & QStyle::State_MouseOver;
    pressed = state & QStyle::State_Sunken;
    hasFocus = state & QStyle::State_HasFocus;
}

bool isDarkMode(const QPalette& pal) {
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

} // namespace Orchid
