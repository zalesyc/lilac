#include "orchid.h"

namespace Orchid {

using CRole = QPalette::ColorRole;
using CGroup = QPalette::ColorGroup;

QColor getColor(const QPalette& pal, const Color color) {
    switch (color) {
        case outline: {
            const auto base = pal.color(CGroup::Normal, CRole::Base);
            return isDarkMode(pal) ? base.lighter(150) : base.darker(150);
        }

        case buttonBackground:
            return pal.color(CGroup::Normal, CRole::Button);

        case disabledButtonBackground:
            return pal.color(CGroup::Disabled, CRole::Button);

        case buttonHover: {
            const auto base = getColor(pal, buttonBackground);
            return isDarkMode(pal) ? base.lighter(120) : base.darker(120);
        }
        case buttonClicked: {
            const auto base = getColor(pal, buttonBackground);
            return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
        }
        case buttonChecked:
            return pal.color(CGroup::Normal, CRole::Accent);

        case checkBoxCheck:
        case disabledCheckBoxCheck:
            return pal.color(CGroup::Normal, CRole::Window);

        case checkBoxOutline: {
            const auto base = pal.color(CGroup::Normal, CRole::Text);
            return isDarkMode(pal) ? base.darker(120) : base.lighter(120);
        }
        case checkBoxInside:
            return isDarkMode(pal) ? QColor(128, 200, 148) : QColor(14, 156, 87);

        case disabledCheckBoxInside:
        case disabledCheckBoxOutline:
            return pal.color(CGroup::Disabled, CRole::Text);

        case tabCheckedFill:
            return getColor(pal, Color::tabWidgetPageArea);

        case tabCheckedOutline:
            return getColor(pal, Color::outline);

        case tabUncheckedHover:
            return getColor(pal, tabCheckedFill);

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
            return isDarkMode(pal) ? base.lighter(130) : base.darker(130);
        }
        case scrollbarSliderHover: {
            const auto base = getColor(pal, Color::scrollBarSlider);
            return isDarkMode(pal) ? base.lighter(140) : base.darker(140);
        }
        case scrollbarSliderClick: {
            const auto base = getColor(pal, Color::scrollBarSlider);
            return isDarkMode(pal) ? base.lighter(170) : base.darker(170);
        }
        case sliderHandle:
            return pal.color(CGroup::Normal, CRole::Accent);

        case sliderHandleHoverCircle: {
            auto base = pal.color(CGroup::Normal, CRole::Accent);
            base.setAlpha(50);
            return base;
        }

        case sliderHandleHoverCircleClick: {
            auto base = pal.color(CGroup::Normal, CRole::Accent);
            base.setAlpha(80);
            return base;
        }

        case sliderHandleDisabled:
            return pal.color(CGroup::Disabled, CRole::Accent);

        case sliderLineBefore:
            return pal.color(CGroup::Normal, CRole::Accent);

        case sliderLineAfter: {
            const auto base = getColor(pal, sliderLineBeforeDisabled);
            return isDarkMode(pal) ? base.lighter(180) : base.darker(180);
        }

        case sliderLineBeforeDisabled:
            return pal.color(CGroup::Disabled, CRole::Accent);

        case sliderLineAfterDisabled: {
            const auto base = getColor(pal, sliderLineBeforeDisabled);
            return isDarkMode(pal) ? base.darker(110) : base.lighter(110);
        }

        case sliderTickmarks:
            return getColor(pal, sliderLineAfter);

        case focusColor:
            return pal.color(CGroup::Normal, CRole::Highlight);

        default:
            return QColor::fromString("red");
    }
}

QBrush getBrush(const QPalette& pal, const Color color) {
    return QBrush(getColor(pal, color));
}

bool isDarkMode(const QPalette& pal) {
    return pal.color(QPalette::ColorRole::Window).lightness() < pal.color(QPalette::ColorRole::Text).lightness();
}

} // namespace Orchid
