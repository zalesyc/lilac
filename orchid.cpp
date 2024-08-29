#include "orchid.h"

namespace Orchid {

static QColor getColorFromPallete(const QPalette& pal, const Color color, const State& state);
static QColor getColorHardcoded(const QPalette& pal, const Color color, const State& state);

using CRole = QPalette::ColorRole;
using CGroup = QPalette::ColorGroup;

const QColor getColor(const QPalette& pal, const Color color, const State& state) {
    return getColorHardcoded(pal, color, state);
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

static QColor getColorHardcoded(const QPalette& pal, const Color color, const State& state) {
    switch (color) {
        case outline:
            return QColor(58, 58, 58);
        case button: {
            if (!state.enabled)
                return QColor(143, 143, 143);
            if (state.pressed)
                return QColor(84, 84, 84);
            if (state.hovered)
                return QColor(60, 60, 60);
            return QColor(42, 42, 42);
        }
        case toggleButtonChecked:
            return pal.color(CGroup::Normal, CRole::Accent);

        case checkBoxCheck:
            return pal.color(CGroup::Normal, CRole::Window);

        case checkBoxOutline: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            const auto base = pal.color(CGroup::Normal, CRole::Text);
            return isDarkMode(pal) ? base.darker(120) : base.lighter(120);
        }

        case checkBoxInside:
            return QColor(129, 201, 149);

        default:
            break;
    }
    return getColorFromPallete(pal, color, state); // TODO: remove this fallback and make This function the fallback
}

static QColor getColorFromPallete(const QPalette& pal, const Color color, const State& state) {
    switch (color) {
        case outline: {
            const auto base = pal.color(CGroup::Normal, CRole::Base);
            return isDarkMode(pal) ? base.lighter(150) : base.darker(150);
        }
        case button: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Button);

            const auto base = pal.color(CGroup::Normal, CRole::Button);
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
            return base;
        }

        case toggleButtonChecked:
            return pal.color(CGroup::Normal, CRole::Accent);

        case checkBoxCheck:
            return pal.color(CGroup::Normal, CRole::Window);

        case checkBoxOutline: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            const auto base = pal.color(CGroup::Normal, CRole::Text);
            return isDarkMode(pal) ? base.darker(120) : base.lighter(120);
        }
        case checkBoxInside: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            return isDarkMode(pal) ? QColor(129, 201, 149) : QColor(14, 156, 87);
        }

        case tabCheckedFill:
            return getColor(pal, Color::tabWidgetPageArea);

        case tabCheckedOutline:
            return getColor(pal, Color::outline);

        case tabUncheckedHover:
            return getColor(pal, Color::tabCheckedFill);

        case tabWidgetPageArea:
            return pal.color(CGroup::Normal, CRole::Base);

        case scrollBarHoverBackground: {
            const auto base = pal.color(CGroup::Normal, CRole::Base);
            return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
        }

        case scrollBarHoverOutline:
            return getColor(pal, Color::outline);

        case scrollBarSlider: {
            const auto base = getColor(pal, Color::scrollBarHoverBackground);
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(170) : base.darker(170);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
            return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
        }

        case sliderHandle:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Accent);
            return pal.color(CGroup::Normal, CRole::Accent);

        case sliderHandleHoverCircle: {
            auto base = pal.color(CGroup::Normal, CRole::Accent);
            if (state.pressed) {
                base.setAlpha(80);
                return base;
            }
            base.setAlpha(50);
            return base;
        }

        case sliderLineBefore:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Accent);
            return pal.color(CGroup::Normal, CRole::Accent);

        case sliderLineAfter: {
            const auto base = pal.color(CGroup::Disabled, CRole::Accent);
            if (!state.enabled)
                return isDarkMode(pal) ? base.darker(110) : base.lighter(110);
            return isDarkMode(pal) ? base.lighter(180) : base.darker(180);
        }
        case sliderTickmarks:
            return getColor(pal, sliderLineAfter);

        case lineEditBackground:
        case spinBoxBackground:
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Button);
            return pal.color(CGroup::Normal, CRole::Button);

        case lineEditOutline:
        case spinBoxOutline:
            if (state.hasFocus)
                return pal.color(CGroup::Normal, CRole::Accent);
            return getColor(pal, lineEditBackground, state);

        case spinBoxIndicator: {
            if (!state.enabled)
                return pal.color(CGroup::Disabled, CRole::Text);
            const QColor base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed)
                return isDarkMode(pal) ? base.lighter(150) : base.darker(150);
            if (state.hovered)
                return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
            return base;
        }
        case spinBoxIndicatorHoverCircle: {
            QColor base = pal.color(CGroup::Normal, CRole::Text);
            if (state.pressed) {
                base.setAlpha(100);
                return base;
            }
            if (state.hovered) {
                base.setAlpha(40);
                return base;
            }
        }
        case focusColor:
            return pal.color(CGroup::Normal, CRole::Highlight);

        default:
            return QColor::fromString("red");
    }
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

} // namespace Orchid
