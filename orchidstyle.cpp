#include "orchidstyle.h"
#include "orchid.h"

#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QStyleFactory>
#include <cmath>

namespace Orchid {

void Style::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
    switch (control) {
        case CC_ScrollBar:
            if (const QStyleOptionSlider* bar = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {

                QStyleOptionSlider sliderOption(*bar);
                sliderOption.rect = subControlRect(control, opt, SC_ScrollBarGroove, widget);

                QStyleOption subOption(*opt);
                subOption.rect = subControlRect(control, &sliderOption, SC_ScrollBarAddPage, widget);
                drawControl(QStyle::CE_ScrollBarAddPage, &subOption, p, widget);

                subOption.rect = subControlRect(control, &sliderOption, SC_ScrollBarSubPage, widget);
                drawControl(QStyle::CE_ScrollBarSubPage, &subOption, p, widget);

                subOption.rect = subControlRect(control, &sliderOption, SC_ScrollBarSlider, widget);
                drawControl(QStyle::CE_ScrollBarSlider, &subOption, p, widget);

                return;
            }

            break;
        default:
            break;
    }
    SuperStyle::drawComplexControl(control, opt, p, widget);
}

void Style::drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const {
    switch (element) {
        case CE_PushButtonBevel:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
                QRect br = btn->rect;
                int dbi = proxy()->pixelMetric(PM_ButtonDefaultIndicator, btn, widget);
                if (btn->features & QStyleOptionButton::DefaultButton)
                    proxy()->drawPrimitive(PE_FrameDefaultButton, opt, p, widget);
                if (btn->features & QStyleOptionButton::AutoDefaultButton)
                    br.setCoords(br.left() + dbi, br.top() + dbi, br.right() - dbi, br.bottom() - dbi);

                QStyleOptionButton tmpBtn = *btn;
                tmpBtn.rect = br;
                proxy()->drawPrimitive(PE_PanelButtonCommand, btn, p, widget);

                if (btn->features & QStyleOptionButton::HasMenu) {
                    int mbi = proxy()->pixelMetric(PM_MenuButtonIndicator, btn, widget);
                    QRect ir = btn->rect;
                    QStyleOptionButton newBtn = *btn;
                    newBtn.rect = QRect(ir.right() - mbi - 2, ir.height() / 2 - mbi / 2 + 3, mbi - 6, mbi - 6);
                    newBtn.rect = visualRect(btn->direction, br, newBtn.rect);
                    proxy()->drawPrimitive(PE_IndicatorArrowDown, &newBtn, p, widget);
                }
                return;
            }
            break;
        case CE_TabBarTabShape:
            if (const auto* tab = qstyleoption_cast<const QStyleOptionTab*>(opt)) {
                if (!(tab->state & (QStyle::State_Selected | QStyle::State_HasFocus | QStyle::State_Sunken | QStyle::State_MouseOver))) {
                    return;
                }
                if (!(tab->state & (QStyle::State_Enabled | QStyle::State_Selected | QStyle::State_HasFocus)) && tab->state & (QStyle::State_Sunken | QStyle::State_MouseOver)) {
                    // this is for when the widget is disabled, to not show anything on hover
                    return;
                }

                QRect rect;
                if (tab->state & (QStyle::State_Selected | QStyle::State_HasFocus)) {
                    rect = tab->rect.adjusted(1, 1, -1, 1);
                } else {
                    rect = tab->rect;
                }

                int cornerRectSize;
                switch (tab->shape) {
                    case QTabBar::RoundedNorth:
                    case QTabBar::TriangularNorth:
                    case QTabBar::RoundedSouth:
                    case QTabBar::TriangularSouth:
                        cornerRectSize = std::min({Constants::btnRadius * 2, // the corner radius is still btnRadius,
                                                   rect.width(),             // it has to be *2 due to implementation
                                                   rect.height() * 2});
                        break;

                    case QTabBar::RoundedEast:
                    case QTabBar::TriangularEast:
                    case QTabBar::RoundedWest:
                    case QTabBar::TriangularWest:
                        cornerRectSize = std::min({Constants::btnRadius * 2,
                                                   rect.height(),
                                                   rect.width() * 2});
                        break;
                };

                QPainterPath path;
                switch (tab->shape) {
                    case QTabBar::RoundedNorth:
                    case QTabBar::TriangularNorth:
                        path.moveTo(rect.bottomLeft());
                        path.lineTo(rect.left(), rect.top() + Constants::btnRadius);
                        path.arcTo(QRectF(
                                       rect.topLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.top() + cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.right() - Constants::btnRadius, rect.top());
                        path.arcTo(QRectF(
                                       rect.topRight(),
                                       QPointF(rect.right() - cornerRectSize, rect.top() + cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.bottomRight());
                        break;

                    case QTabBar::RoundedSouth:
                    case QTabBar::TriangularSouth:
                        path.moveTo(rect.topLeft());
                        path.lineTo(rect.left(), rect.bottom() - Constants::btnRadius);
                        path.arcTo(QRectF(
                                       rect.bottomLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.bottom() - cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.right() - Constants::btnRadius, rect.bottom());
                        path.arcTo(QRectF(
                                       rect.bottomRight(),
                                       QPointF(rect.right() - cornerRectSize, rect.bottom() - cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.topRight());
                        break;

                    case QTabBar::RoundedEast:
                    case QTabBar::TriangularEast:
                        path.moveTo(rect.topLeft());
                        path.lineTo(rect.right() - Constants::btnRadius, rect.top());
                        path.arcTo(QRectF(
                                       rect.topRight(),
                                       QPointF(rect.right() - cornerRectSize, rect.top() + cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.right(), rect.bottom() - Constants::btnRadius);
                        path.arcTo(QRectF(
                                       rect.bottomRight(),
                                       QPointF(rect.right() - cornerRectSize, rect.bottom() - cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.bottomLeft());
                        break;

                    case QTabBar::RoundedWest:
                    case QTabBar::TriangularWest:
                        path.moveTo(rect.topRight());
                        path.lineTo(rect.left() + Constants::btnRadius, rect.top());
                        path.arcTo(QRectF(
                                       rect.topLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.top() + cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.left(), rect.bottom() - Constants::btnRadius);
                        path.arcTo(QRectF(
                                       rect.bottomLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.bottom() - cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.bottomRight());
                        break;
                }
                p->save();
                p->setRenderHints(QPainter::Antialiasing);

                if (tab->state & (QStyle::State_Selected | QStyle::State_HasFocus)) {
                    p->setPen(QPen(getBrush(tab->palette, Color::tabCheckedOutline), 1));
                    p->setBrush(getBrush(tab->palette, Color::tabCheckedFill));
                    p->drawPath(path);
                } else {
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(tab->palette, Color::tabUncheckedHover));
                    p->drawPath(path);
                }

                p->restore();
                return;
            }
            break;

        case CE_ScrollBarAddPage: {
            const QRect rect = opt->rect;

            p->save();
            p->setPen(QPen(getBrush(opt->palette, Color::scrollBarHoverOutline), 1));
            p->setBrush(Qt::NoBrush);
            p->fillRect(rect, getBrush(opt->palette, Color::scrollBarHoverBackground));

            if (opt->state & QStyle::State_Horizontal) {
                p->drawLine(rect.topLeft(), rect.topRight());
                p->drawLine(rect.bottomLeft(), rect.bottomRight());
            } else {
                p->drawLine(rect.topLeft(), rect.bottomLeft());
                p->drawLine(rect.topRight(), rect.bottomRight());
            }

            p->restore();

            return;
        }

        case CE_ScrollBarSubPage:
            drawControl(QStyle::CE_ScrollBarAddPage, opt, p, widget);
            return;

        case CE_ScrollBarSlider: {
            drawControl(QStyle::CE_ScrollBarAddPage, opt, p, widget);
            QRect rect;
            if (opt->state & QStyle::State_Horizontal) {
                const int gapHeigth = std::floor(opt->rect.height() / 4.0);
                rect = opt->rect.adjusted(0, gapHeigth, 0, -gapHeigth);
            } else {
                const int gapWidth = std::floor(opt->rect.width() / 4.0);
                rect = opt->rect.adjusted(gapWidth, 0, -gapWidth, 0);
            }
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            if (opt->state & QStyle::State_Sunken) {
                p->setBrush(getBrush(opt->palette, Color::scrollbarSliderClick));
            } else if (opt->state & QStyle::State_MouseOver) {
                p->setBrush(getBrush(opt->palette, Color::scrollbarSliderHover));
            } else {
                p->setBrush(getBrush(opt->palette, Color::scrollBarSlider));
            }

            const double rectSize = (opt->state & QStyle::State_Horizontal) ? rect.height() / 2.0 : rect.width() / 2.0;
            p->drawRoundedRect(rect, rectSize, rectSize);
            p->restore();

            return;
        }

        default:
            break;
    }
    SuperStyle::drawControl(element, opt, p, widget);
}

void Style::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const {
    switch (element) {
        case PE_PanelButtonCommand:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
                p->save();
                p->setRenderHints(QPainter::Antialiasing);
                p->setPen(Qt::NoPen);
                if (btn->state & QStyle::State_On) {
                    p->setBrush(getBrush(btn->palette, (btn->state & QStyle::State_Enabled) ? Color::buttonChecked : Color::disabledButtonBackground));
                } else if (btn->state & QStyle::State_Sunken) {
                    p->setBrush(getBrush(btn->palette, Color::buttonClicked));
                } else if (btn->state & QStyle::State_MouseOver) {
                    p->setBrush(getBrush(btn->palette, Color::buttonHover));
                } else if (btn->features & QStyleOptionButton::Flat) {
                    p->setBrush(Qt::NoBrush);
                } else if (!btn->state & QStyle::State_Enabled) {
                    p->setBrush(getBrush(btn->palette, Color::disabledButtonBackground));
                } else {
                    p->setBrush(getBrush(btn->palette, Color::buttonBackground));
                }

                p->drawRoundedRect(btn->rect, Constants::btnRadius, Constants::btnRadius);
                p->restore();
                return;
            }
            break;
        case PE_FrameFocusRect:
            // this also has QStyleOptionFocusRect - in case of bugs look here
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setBrush(Qt::NoBrush);
            p->setPen(QPen(getBrush(opt->palette, Color::focusColor), 2));
            p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), Constants::btnRadius, Constants::btnRadius);
            p->restore();
            return;

        case PE_IndicatorCheckBox:
        case PE_IndicatorRadioButton: {
            p->save();
            p->setRenderHints(QPainter::Antialiasing);

            bool enabled = opt->state & State_Enabled;

            if (opt->state & (QStyle::State_Off)) {
                p->setPen(QPen(getBrush(opt->palette, enabled ? Color::checkBoxOutline : Color::disabledCheckBoxOutline), 2));
                p->setBrush(Qt::NoBrush);
                p->drawChord(opt->rect.adjusted(1, 1, -1, -1), 0, 16 * 360); // the angle is in 1/16th of a degree
                p->restore();
                return;
            }

            p->setPen(Qt::NoPen);
            p->setBrush(getBrush(opt->palette, enabled ? Color::checkBoxInside : Color::disabledCheckBoxInside));
            p->drawChord(opt->rect, 0, 16 * 360); // the angle is in 1/16th of a degree

            const int smallerSide = std::min(opt->rect.width(), opt->rect.height());

            if (element == QStyle::PE_IndicatorCheckBox) {
                p->setPen(QPen(getBrush(opt->palette, enabled ? Color::checkBoxCheck : Color::disabledCheckBoxCheck), 2));
                p->setBrush(Qt::NoBrush);

                const double widthOffset = opt->rect.width() - smallerSide;

                if (opt->state & QStyle::State_NoChange) {
                    const double adjustment = smallerSide / 4.0f;
                    const double y = opt->rect.center().y() + 1;

                    p->drawLine(QLineF(opt->rect.left() + widthOffset + adjustment + 1, y, opt->rect.right() - widthOffset - adjustment, y));

                } else {
                    const auto center = opt->rect.center();
                    const QPointF poinsts[3] = {
                        QPointF(center.x() - smallerSide / 4.0f, center.y()),
                        QPointF(center.x() - smallerSide / 10.0f, center.y() + smallerSide / 5.0f),
                        QPointF(center.x() + smallerSide / 4.0f, center.y() - smallerSide / 7.5f),
                    };

                    p->drawPolyline(poinsts, 3);
                }
            } else {
                const double adjustment = smallerSide * 0.35;

                p->setPen(Qt::NoPen);
                p->setBrush(getBrush(opt->palette, enabled ? Color::checkBoxCheck : Color::disabledCheckBoxCheck));
                p->drawChord(opt->rect.toRectF().adjusted(adjustment, adjustment, -adjustment, -adjustment), 0, 16 * 3600);
            }

            p->restore();
            return;
        }
        case PE_FrameTabWidget:
            p->save();
            p->setBrush(getBrush(opt->palette, Color::tabWidgetPageArea));
            p->setPen(QPen(getBrush(opt->palette, Color::tabCheckedOutline), 1));
            p->drawRect(opt->rect.adjusted(1, 1, -1, -1));
            p->restore();
            return;

        case PE_FrameTabBarBase:
            p->save();
            p->setBrush(Qt::NoBrush);
            p->setPen(QPen(getBrush(opt->palette, Color::tabCheckedOutline), 1));
            p->drawRect(opt->rect);
            p->restore();
            return;

        default:
            break;
    }
    SuperStyle::drawPrimitive(element, opt, p, widget);
}
void Style::polish(QWidget* widget) {
    SuperStyle::polish(widget);
    if (widget->inherits("QAbstractButton") || widget->inherits("QTabBar") || widget->inherits("QScrollBar")) {
        widget->setAttribute(Qt::WA_Hover, true);
    }
}

void Style::unpolish(QWidget* widget) {
    SuperStyle::unpolish(widget);
    if (widget->inherits("QAbstractButton") || widget->inherits("QTabBar") || widget->inherits("QScrollBar")) {
        widget->setAttribute(Qt::WA_Hover, false);
    }
}

int Style::pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt, const QWidget* widget) const {
    switch (m) {
        case PM_ButtonShiftHorizontal:
        case PM_ButtonShiftVertical:
            return 0;

        case PM_IndicatorHeight:
        case PM_IndicatorWidth:
        case PM_ExclusiveIndicatorWidth:
        case PM_ExclusiveIndicatorHeight:
            return 20;
        case PM_TabBarTabVSpace:
            return 20;
        case PM_TabBarTabHSpace:
            return 24;
        case PM_TabBarTabShiftHorizontal:
        case PM_TabBarTabShiftVertical:
            return 0;
        default:
            break;
    }
    return SuperStyle::pixelMetric(m, opt, widget);
}

QRect Style::subElementRect(QStyle::SubElement element, const QStyleOption* opt, const QWidget* widget) const {
    switch (element) {
        case SE_PushButtonFocusRect:
            return opt->rect;
            break;
        default:
            break;
    }
    return SuperStyle::subElementRect(element, opt, widget);
}

QRect Style::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl element, const QWidget* widget) const {
    switch (element) {
        case SC_ScrollBarGroove:
            return opt->rect;

        default:
            break;
    }
    return SuperStyle::subControlRect(cc, opt, element, widget);
}

const std::optional<const QString> Style::getStyle() { // this is not ideal, but shold work - todo: make this configurable in settings
    const QStringList availibleStyles = QStyleFactory::keys();
    if (availibleStyles.contains("breeze", Qt::CaseInsensitive)) {
        return QString("Breeze");
    }
    if (availibleStyles.contains("fusion", Qt::CaseInsensitive)) {
        return QString("Fusion");
    }
    return std::nullopt;
}

} // namespace Orchid
