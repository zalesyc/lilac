#include "orchidstyle.h"
#include "orchid.h"

#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QStyleFactory>
#include <QtMath>
#include <cmath>
namespace Orchid {

void Style::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
    const Orchid::State state(opt->state); // this hat to be defined as Orchid::State because just State would conflict with State from QStyle
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
        case CC_Slider:
            if (const auto* slider = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                const QRect grooveRect = subControlRect(QStyle::CC_Slider, slider, QStyle::SC_SliderGroove, widget);
                const QRect handleHoverRect = subControlRect(QStyle::CC_Slider, slider, QStyle::SC_SliderHandle, widget);
                QRect handleRect;
                handleRect.setWidth(Constants::sliderHandleCircleDiameter);
                handleRect.setHeight(Constants::sliderHandleCircleDiameter);
                handleRect.moveCenter(handleHoverRect.center());

                const int len = slider->orientation == Qt::Horizontal ? handleHoverRect.width() : handleHoverRect.height();

                p->save();
                p->setRenderHint(QPainter::Antialiasing);

                if (slider->orientation == Qt::Horizontal) {
                    if (slider->upsideDown) {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineAfter, state), 2));
                    } else {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineBefore, state), 2));
                    }
                    p->drawLine(grooveRect.left() + (len / 2),
                                grooveRect.center().y() + 1,
                                handleRect.center().x(),
                                grooveRect.center().y() + 1);

                    if (slider->upsideDown) {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineBefore, state), 2));
                    } else {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineAfter, state), 2));
                    }
                    p->drawLine(handleRect.center().x(),
                                grooveRect.center().y() + 1,
                                grooveRect.right() - (len / 2),
                                grooveRect.center().y() + 1);
                } else {
                    if (slider->upsideDown) {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineAfter, state), 2));
                    } else {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineBefore, state), 2));
                    }
                    p->drawLine(grooveRect.center().x() + 1,
                                grooveRect.top() + (len / 2),
                                grooveRect.center().x() + 1,
                                handleRect.center().y());

                    if (slider->upsideDown) {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineBefore, state), 2));
                    } else {
                        p->setPen(QPen(getBrush(slider->palette, sliderLineAfter, state), 2));
                    }
                    p->drawLine(grooveRect.center().x() + 1,
                                handleRect.center().y(),
                                grooveRect.center().x() + 1,
                                grooveRect.bottom() - (len / 2));
                }

                if (slider->subControls & SC_SliderTickmarks) {
                    // ------------ tickmarks
                    const int tickOffset = this->pixelMetric(PM_SliderTickmarkOffset, slider, widget);
                    int interval = slider->tickInterval;
                    if (interval <= 0)
                        interval = 10;
                    QList<QLine> lines;
                    if (slider->tickPosition == QSlider::TicksBothSides) {
                        QStyleOptionSlider customOpt(*slider);
                        QRect tickmarksRect = subControlRect(QStyle::CC_Slider, slider, QStyle::SC_SliderTickmarks, widget);
                        // tickmarksRect is initially the bottom/right one and then gets moved to the top

                        // bottom/right ticks
                        this->sliderGetTickmarks(&lines, slider, tickmarksRect, len, interval);

                        // top/left ticks
                        if (slider->orientation == Qt::Horizontal) {
                            tickmarksRect.moveTop(slider->rect.top());
                        } else {
                            tickmarksRect.moveLeft(slider->rect.left());
                        }
                        this->sliderGetTickmarks(&lines, slider, tickmarksRect, len, interval);

                    } else {
                        const QRect tickmarksRect = subControlRect(QStyle::CC_Slider, slider, QStyle::SC_SliderTickmarks, widget);
                        this->sliderGetTickmarks(&lines, slider, tickmarksRect, len, interval);
                    }
                    p->setRenderHint(QPainter::Antialiasing, false);
                    p->setPen(QPen(getBrush(slider->palette, sliderTickmarks), 1));
                    p->drawLines(lines);
                    p->setRenderHint(QPainter::Antialiasing);

                    // ------------- slider
                    const QRectF handleRectF(handleRect);
                    // TODO: remove the std::acos func as it is expensive - the parameters are constants
                    // from PixelMetric so it should be replaced by a precalculated value
                    const qreal handleRadius = handleRectF.height() / 2.0;
                    const qreal targentsAngle = qRadiansToDegrees(std::acos(handleRadius /
                                                                            (handleRadius + tickOffset)));
                    const qreal circleRestAngle = 360 - (2 * targentsAngle);
                    const qreal circleBetweenTargentsAngle = 180 - (2 * targentsAngle);

                    QPainterPath path;
                    if (slider->orientation == Qt::Horizontal) {
                        switch (slider->tickPosition) {
                            case QSlider::TicksAbove:
                                path.moveTo(handleRectF.center().x(), handleRectF.top() - tickOffset);
                                path.arcTo(handleRectF, 90 - targentsAngle, -circleRestAngle);
                                break;
                            case QSlider::TicksBothSides:
                                path.moveTo(handleRectF.center().x(), handleRectF.top() - tickOffset);
                                path.arcTo(handleRectF, 90 - targentsAngle, -circleBetweenTargentsAngle);
                                path.lineTo(handleRectF.center().x(), handleRectF.bottom() + tickOffset);
                                path.arcTo(handleRectF, 270 - targentsAngle, -circleBetweenTargentsAngle);
                                break;
                            case QSlider::NoTicks:
                            case QSlider::TicksBelow:
                                path.moveTo(handleRectF.center().x(), handleRectF.bottom() + tickOffset);
                                path.arcTo(handleRectF, 270 - targentsAngle, -circleRestAngle);
                                break;
                        }
                    } else {
                        switch (slider->tickPosition) {
                            case QSlider::TicksLeft:
                                path.moveTo(handleRectF.left() - tickOffset, handleRectF.center().y());
                                path.arcTo(handleRectF, 180 - targentsAngle, -circleRestAngle);
                                break;
                            case QSlider::TicksBothSides:
                                path.moveTo(handleRectF.left() - tickOffset, handleRectF.center().y());
                                path.arcTo(handleRectF, 180 - targentsAngle, -circleBetweenTargentsAngle);
                                path.lineTo(handleRectF.right() + tickOffset, handleRectF.center().y());
                                path.arcTo(handleRectF, 360 - targentsAngle, -circleBetweenTargentsAngle);
                                break;
                            case QSlider::NoTicks:
                            case QSlider::TicksRight:
                                path.moveTo(handleRectF.right() + tickOffset, handleRectF.center().y());
                                path.arcTo(handleRectF, 360 - targentsAngle, -circleRestAngle);
                                break;
                        }
                    }

                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(slider->palette, sliderHandle, state));
                    p->drawPath(path);
                } else {
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(slider->palette, sliderHandle, state));
                    p->drawEllipse(handleRect);
                }

                // hover circle
                if (state.hovered && state.enabled) {
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(slider->palette, sliderHandleHoverCircle, state));
                    p->drawEllipse(handleHoverRect);
                }
                p->restore();
                return;
            }
            break;
        case CC_SpinBox:
            if (const auto* spin = qstyleoption_cast<const QStyleOptionSpinBox*>(opt)) {
                if (spin->frame && (spin->subControls & SC_SpinBoxFrame)) {
                    p->save();
                    p->setRenderHint(QPainter::Antialiasing);
                    p->setPen(QPen(getBrush(spin->palette, Color::spinBoxOutline, state), 2));
                    p->setBrush(getBrush(spin->palette, Color::spinBoxBackground, state));
                    p->drawRoundedRect(spin->rect.adjusted(1, 1, -1, -1), Constants::btnRadius, Constants::btnRadius);
                    p->restore();
                }

                if (spin->subControls & SC_SpinBoxUp) {
                    QStyleOptionSpinBox copy(*spin);
                    copy.subControls = SC_SpinBoxUp;
                    if (!(spin->stepEnabled & QAbstractSpinBox::StepUpEnabled)) {
                        copy.state &= ~State_Enabled;
                    }

                    if (spin->activeSubControls != SC_SpinBoxUp) {
                        copy.state &= ~State_Sunken;
                        copy.state &= ~State_MouseOver;
                    }
                    const auto pe = (spin->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinPlus : PE_IndicatorSpinUp);

                    copy.rect = subControlRect(CC_SpinBox, spin, SC_SpinBoxUp, widget);
                    drawPrimitive(pe, &copy, p, widget);
                }

                if (spin->subControls & SC_SpinBoxDown) {
                    QStyleOptionSpinBox copy(*spin);
                    copy.subControls = SC_SpinBoxDown;
                    if (!(spin->stepEnabled & QAbstractSpinBox::StepDownEnabled)) {
                        copy.state &= ~State_Enabled;
                    }
                    if (spin->activeSubControls != SC_SpinBoxDown) {
                        copy.state &= ~State_Sunken;
                        copy.state &= ~State_MouseOver;
                    }
                    const auto pe = (spin->buttonSymbols == QAbstractSpinBox::PlusMinus ? PE_IndicatorSpinMinus : PE_IndicatorSpinDown);

                    copy.rect = subControlRect(CC_SpinBox, spin, SC_SpinBoxDown, widget);
                    drawPrimitive(pe, &copy, p, widget);
                }
                return;
            }
            break;

        default:
            break;
    }
    SuperStyle::drawComplexControl(control, opt, p, widget);
}

void Style::drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const {
    const Orchid::State state(opt->state);
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
            p->setBrush(getBrush(opt->palette, Color::scrollBarSlider, state));

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
    const Orchid::State state(opt->state);
    switch (element) {
        case PE_PanelButtonCommand:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
                p->save();
                p->setRenderHints(QPainter::Antialiasing);
                p->setPen(Qt::NoPen);
                if (btn->state & QStyle::State_On && state.enabled) {
                    p->setBrush(getBrush(btn->palette, Color::toggleButtonChecked));
                } else if (btn->features & QStyleOptionButton::Flat && !state.hovered && !state.hovered) {
                    p->setBrush(Qt::NoBrush);
                } else {
                    p->setBrush(getBrush(btn->palette, Color::button, state));
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
            if (opt->state & (QStyle::State_Off)) {
                p->setPen(QPen(getBrush(opt->palette, Color::checkBoxOutline, state), 2));
                p->setBrush(Qt::NoBrush);
                p->drawChord(opt->rect.adjusted(1, 1, -1, -1), 0, 16 * 360); // the angle is in 1/16th of a degree
                p->restore();
                return;
            }

            p->setPen(Qt::NoPen);
            p->setBrush(getBrush(opt->palette, Color::checkBoxInside, state));
            p->drawChord(opt->rect, 0, 16 * 360); // the angle is in 1/16th of a degree

            const int smallerSide = std::min(opt->rect.width(), opt->rect.height());

            if (element == QStyle::PE_IndicatorCheckBox) {
                p->setPen(QPen(getBrush(opt->palette, Color::checkBoxCheck, state), 2));
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
                p->setBrush(getBrush(opt->palette, Color::checkBoxCheck, state));
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

        case PE_PanelLineEdit:
            if (const QStyleOptionFrame* edit = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
                if (edit->lineWidth > 0) {
                    p->save();
                    p->setRenderHints(QPainter::Antialiasing);
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(edit->palette, Color::lineEditBackground, state));
                    p->drawRoundedRect(opt->rect, Constants::btnRadius, Constants::btnRadius);
                    p->restore();
                    this->drawPrimitive(PE_FrameLineEdit, edit, p, widget);
                }
                return;
            }
            break;

        case PE_FrameLineEdit:
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(QPen(getBrush(opt->palette, Color::lineEditOutline, state), 2));
            p->setBrush(Qt::NoBrush);
            p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), Constants::btnRadius, Constants::btnRadius);
            p->restore();
            return;

        case PE_IndicatorSpinUp:
        case PE_IndicatorSpinPlus:
        case PE_IndicatorSpinDown:
        case PE_IndicatorSpinMinus:
            if (const auto* spin = qstyleoption_cast<const QStyleOptionSpinBox*>(opt)) {
                if ((state.hovered || state.pressed) && state.enabled) {
                    const int outlineSize = qMin(
                        qMin(spin->rect.width(), spin->rect.height()),
                        Constants::spinIndicatorHoverCircleSize);

                    p->save();
                    p->setRenderHints(QPainter::Antialiasing);
                    p->setPen(Qt::NoPen);
                    p->setBrush(getColor(spin->palette, Color::spinBoxIndicatorHoverCircle, state));
                    p->drawEllipse(spin->rect.center() + QPoint(1, 1), outlineSize / 2, outlineSize / 2);
                    p->restore();
                }
                const int indicatorSize = qMin(
                    qMin(spin->rect.width(), spin->rect.height()),
                    Constants::spinIndicatorSize);
                QRect indicatorRect(0, 0, indicatorSize, indicatorSize);
                indicatorRect.moveCenter(spin->rect.center());

                p->save();
                p->setBrush(Qt::NoBrush);
                p->setPen(QPen(QBrush(getColor(spin->palette, Color::spinBoxIndicator, state)), 2));
                p->drawLine(indicatorRect.left() + 1, indicatorRect.center().y() + 1, indicatorRect.right(), indicatorRect.center().y() + 1);
                if (element == QStyle::PE_IndicatorSpinUp || element == QStyle::PE_IndicatorSpinPlus)
                    p->drawLine(indicatorRect.center().x() + 1, indicatorRect.top() + 1, indicatorRect.center().x() + 1, indicatorRect.bottom());
                p->restore();
                return;
            }
            break;

        default:
            break;
    }
    SuperStyle::drawPrimitive(element, opt, p, widget);
}
void Style::polish(QWidget* widget) {
    SuperStyle::polish(widget);
    if (widget->inherits("QAbstractButton") || widget->inherits("QTabBar") || widget->inherits("QScrollBar") || widget->inherits("QAbstractSlider") || widget->inherits("QAbstractSpinBox")) {
        widget->setAttribute(Qt::WA_Hover, true);
    }
}

void Style::unpolish(QWidget* widget) {
    SuperStyle::unpolish(widget);
    if (widget->inherits("QAbstractButton") || widget->inherits("QTabBar") || widget->inherits("QScrollBar") || widget->inherits("QAbstractSlider") || widget->inherits("QAbstractSpinBox")) {
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
        case PM_SliderThickness:
        case PM_SliderControlThickness:
        case PM_SliderLength:
            return Constants::sliderHandleHoverCircleDiameter;
        case PM_SliderTickmarkOffset:
            return 3;
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
    switch (cc) {
        case QStyle::CC_Slider:
            if (const QStyleOptionSlider* slider = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                switch (element) {
                    case SC_SliderGroove: {
                        int thickness = this->pixelMetric(PM_SliderControlThickness, slider, widget);
                        QRect rect(slider->rect);
                        if (slider->orientation == Qt::Horizontal) {
                            rect.setHeight(thickness);
                            switch (slider->tickPosition) {
                                case QSlider::TicksAbove:
                                    rect.moveBottom(slider->rect.bottom());
                                    break;
                                case QSlider::TicksBothSides:
                                    rect.moveCenter(slider->rect.center());
                                    break;
                                case QSlider::TicksBelow:
                                case QSlider::NoTicks:
                                    rect.moveTop(slider->rect.top());
                                    break;
                            }

                        } else {
                            rect.setWidth(thickness);
                            switch (slider->tickPosition) {
                                case QSlider::TicksLeft:
                                    rect.moveRight(slider->rect.right());
                                    break;
                                case QSlider::TicksBothSides:
                                    rect.moveCenter(slider->rect.center());
                                    break;
                                case QSlider::TicksRight:
                                case QSlider::NoTicks:
                                    rect.moveLeft(slider->rect.left());
                                    break;
                            }
                        }
                        return rect;
                    }
                    case SC_SliderHandle: {
                        const QRect groove = this->subControlRect(QStyle::CC_Slider, slider, QStyle::SC_SliderGroove, widget);
                        const int len = this->pixelMetric(PM_SliderLength, slider, widget);
                        const int thickness = this->pixelMetric(PM_SliderControlThickness, slider, widget);

                        if (slider->orientation == Qt::Horizontal) {
                            const int pos = sliderPositionFromValue(slider->minimum, slider->maximum, slider->sliderPosition, groove.width() - len, slider->upsideDown);
                            return QRect(groove.left() + pos, groove.top(), len, thickness);
                        } else {
                            const int pos = sliderPositionFromValue(slider->minimum, slider->maximum, slider->sliderPosition, groove.height() - len, slider->upsideDown);
                            return QRect(groove.left(), groove.top() + pos, thickness, len);
                        }
                    }
                    case SC_SliderTickmarks: {
                        const int grooveTicksGap = this->pixelMetric(PM_SliderTickmarkOffset, slider, widget);
                        const QRect grooveRect = this->subControlRect(CC_Slider, slider, SC_SliderGroove, widget);
                        constexpr int handleRadius = Constants::sliderHandleCircleDiameter / 2;
                        QRect rect(slider->rect);
                        // case QSlider::TicksBothSides returns the ticks below, the above ones are handled in DrawComplexControl where they are drawn
                        if (slider->orientation == Qt::Horizontal) {
                            rect.setHeight(Constants::sliderTickmarksLen);
                            switch (slider->tickPosition) {
                                case QSlider::TicksAbove:
                                    rect.moveBottom(grooveRect.center().y() - handleRadius - grooveTicksGap);
                                    break;
                                case QSlider::TicksBothSides:
                                case QSlider::TicksBelow:
                                case QSlider::NoTicks:
                                    rect.moveTop(grooveRect.center().y() + handleRadius + grooveTicksGap);
                                    break;
                            }
                        } else {
                            rect.setWidth(Constants::sliderTickmarksLen);
                            switch (slider->tickPosition) {
                                case QSlider::TicksLeft:
                                    rect.moveRight(grooveRect.center().x() - handleRadius - grooveTicksGap);
                                    break;
                                case QSlider::TicksBothSides:
                                case QSlider::TicksRight:
                                case QSlider::NoTicks:
                                    rect.moveLeft(grooveRect.center().x() + handleRadius + grooveTicksGap);
                                    break;
                            }
                        }
                        return rect;
                    }

                    default:
                        break;
                }
            }
            break;
        case CC_SpinBox:
            if (const auto spin = qstyleoption_cast<const QStyleOptionSpinBox*>(opt)) {
                switch (element) {
                    case SC_SpinBoxFrame:
                        return spin->rect;
                    case SC_SpinBoxUp: {
                        const int width = qMin(spin->rect.width() / 3, Constants::spinBoxIndicatorWidth);
                        QRect rect(spin->rect);
                        rect.setWidth(width);
                        rect.moveRight(spin->rect.right());
                        return rect;
                    }
                    case SC_SpinBoxDown: {
                        const int width = qMin(spin->rect.width() / 3, Constants::spinBoxIndicatorWidth);
                        QRect rect(spin->rect);
                        rect.setWidth(width);
                        rect.moveRight(spin->rect.right() - width);
                        return rect;
                    }
                    case SC_SpinBoxEditField: {
                        const int indicatorWidth = qMin(spin->rect.width() / 3, Constants::spinBoxIndicatorWidth);
                        QRect rect(spin->rect);
                        rect.setLeft(spin->rect.left() + Constants::spinBoxSpacing);
                        rect.setRight(spin->rect.right() - indicatorWidth * 2 - Constants::spinBoxSpacing);
                        return rect;
                    } break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
    switch (element) {
        case SC_ScrollBarGroove:
            return opt->rect;

        default:
            break;
    }
    return SuperStyle::subControlRect(cc, opt, element, widget);
}

QSize Style::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const {
    QSize original = SuperStyle::sizeFromContents(ct, opt, contentsSize, widget);
    switch (ct) {
        case CT_PushButton:
        case CT_LineEdit:
        case CT_SpinBox:
        case CT_ComboBox: {
            const int heigth = qMax(36, original.height());
            original.setHeight(heigth);
        }

        default:
            break;
    }
    return original;
}

const QString Style::getStyle() { // this is not ideal, but shold work - todo: make this configurable in settings
    const QStringList availibleStyles = QStyleFactory::keys();
    if (availibleStyles.contains("breeze", Qt::CaseInsensitive)) {
        return QString("Breeze");
    }

    return QString("Fusion");
}

const void Style::sliderGetTickmarks(QList<QLine>* returnList, const QStyleOptionSlider* slider, const QRect& tickmarksRect, const int sliderLen, const int interval) {
    for (int val = slider->minimum; val < slider->maximum; val += interval) {
        if (slider->orientation == Qt::Horizontal) {
            int pos = sliderPositionFromValue(slider->minimum, slider->maximum, val, tickmarksRect.width() - sliderLen, slider->upsideDown) + (sliderLen / 2);
            *returnList += QLine(pos, tickmarksRect.top() + 0.5, pos, tickmarksRect.bottom() + 0.5);
        } else {
            int pos = sliderPositionFromValue(slider->minimum, slider->maximum, val, tickmarksRect.height() - sliderLen, slider->upsideDown) + (sliderLen / 2);
            *returnList += QLine(tickmarksRect.left() + 0.5, pos, tickmarksRect.right() + 0.5, pos);
        }
    }
}
} // namespace Orchid
