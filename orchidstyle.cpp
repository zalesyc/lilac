#include "orchidstyle.h"
#include "orchid.h"

#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QStyleFactory>
#include <QtMath>
namespace Orchid {

void Style::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const {
    Orchid::State state(opt->state); // this hat to be defined as Orchid::State because just State would conflict with State from QStyle
    switch (control) {
        case CC_ScrollBar:
            if (const QStyleOptionSlider* bar = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                if (state.hovered && state.enabled) {
                    p->save();
                    p->setPen(getPen(opt->palette, Color::scrollBarHoverOutline, 1));
                    p->setBrush(Qt::NoBrush);
                    p->fillRect(bar->rect, getBrush(opt->palette, Color::scrollBarHoverBg));
                    if (opt->state & QStyle::State_Horizontal) {
                        p->drawLine(bar->rect.topLeft(), bar->rect.topRight());
                    } else {
                        p->drawLine(bar->rect.topLeft(), bar->rect.bottomLeft());
                    }
                    p->restore();
                }

                QStyleOptionSlider sliderOption(*bar);
                sliderOption.rect = subControlRect(control, opt, SC_ScrollBarGroove, widget);
                sliderOption.rect = subControlRect(control, &sliderOption, SC_ScrollBarSlider, widget);
                drawControl(QStyle::CE_ScrollBarSlider, &sliderOption, p, widget);
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
                        p->setPen(getPen(slider->palette, sliderLineAfter, state, 2));
                    } else {
                        p->setPen(getPen(slider->palette, sliderLineBefore, state, 2));
                    }
                    p->drawLine(grooveRect.left() + (len / 2),
                                grooveRect.center().y() + 1,
                                handleRect.center().x(),
                                grooveRect.center().y() + 1);

                    if (slider->upsideDown) {
                        p->setPen(getPen(slider->palette, sliderLineBefore, state, 2));
                    } else {
                        p->setPen(getPen(slider->palette, sliderLineAfter, state, 2));
                    }
                    p->drawLine(handleRect.center().x(),
                                grooveRect.center().y() + 1,
                                grooveRect.right() - (len / 2),
                                grooveRect.center().y() + 1);
                } else {
                    if (slider->upsideDown) {
                        p->setPen(getPen(slider->palette, sliderLineAfter, state, 2));
                    } else {
                        p->setPen(getPen(slider->palette, sliderLineBefore, state, 2));
                    }
                    p->drawLine(grooveRect.center().x() + 1,
                                grooveRect.top() + (len / 2),
                                grooveRect.center().x() + 1,
                                handleRect.center().y());

                    if (slider->upsideDown) {
                        p->setPen(getPen(slider->palette, sliderLineBefore, state, 2));
                    } else {
                        p->setPen(getPen(slider->palette, sliderLineAfter, state, 2));
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
                    p->setPen(getPen(slider->palette, sliderTickmarks, 1));
                    p->drawLines(lines);
                    p->setRenderHint(QPainter::Antialiasing);

                    // ------------- slider
                    const QRectF handleRectF(handleRect);
                    // TODO: remove the qAcos func as it is expensive - the parameters are constants
                    // from PixelMetric so it should be replaced by a precalculated value
                    const qreal handleRadius = handleRectF.height() / 2.0;
                    const qreal targentsAngle = qRadiansToDegrees(qAcos(handleRadius / (handleRadius + tickOffset)));
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
                    p->setPen(getPen(spin->palette, Color::spinBoxOutline, state, 2));
                    p->setBrush(getBrush(spin->palette, Color::spinBoxBg, state));
                    p->drawRoundedRect(spin->rect.adjusted(1, 1, -1, -1), Constants::cornerRadius, Constants::cornerRadius);
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

        case CC_ComboBox:
            if (const auto combo = qstyleoption_cast<const QStyleOptionComboBox*>(opt)) {
                state.pressed = (combo->state & State_On);
                p->save();
                p->setRenderHint(QPainter::Antialiasing);
                p->setPen(getPen(combo->palette, Color::comboBoxOutline, state, 2));
                p->setBrush(getBrush(combo->palette, Color::comboBoxBg, state));
                p->drawRoundedRect(combo->rect.adjusted(1, 1, -1, -1), Constants::cornerRadius, Constants::cornerRadius);
                p->restore();

                // arrow
                const QRect arrowAreaRect = this->subControlRect(CC_ComboBox, combo, SC_ComboBoxArrow, widget);
                const int arrowSize = Constants::smallArrowSize;
                QStyleOption arrowOpt(*combo);
                arrowOpt.rect = QRect(0, 0, arrowSize, arrowSize);
                arrowOpt.rect.moveCenter(arrowAreaRect.center());
                this->drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, p, widget);

                return;
            }
            break;
        case CC_ToolButton:
            /* Notes for toolbuttons:
             * - AutoRaise means flat
             * - if (btn->subControls & SC_ToolButtonMenu) coresponds to QToolButton::InstantPopup,
             *   if (btn->features & QStyleOptionToolButton::HasMenu) coresponds to QToolButton::MenuButtonPopup
             */
            if (const auto* btn = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
                if (btn->subControls & SC_ToolButton) {
                    drawPrimitive(PE_PanelButtonTool, btn, p, widget);
                }

                QRect buttonRect = subControlRect(control, btn, SC_ToolButton, widget);

                QStyleOptionToolButton toolOpt = *btn;
                toolOpt.rect = buttonRect;
                drawControl(CE_ToolButtonLabel, &toolOpt, p, widget);

                if (btn->subControls & SC_ToolButtonMenu) {
                    const QRect menuRect = subControlRect(control, btn, SC_ToolButtonMenu, widget);
                    toolOpt.rect = menuRect;
                    drawPrimitive(PE_IndicatorButtonDropDown, &toolOpt, p, widget);
                    toolOpt.rect = QRect(0, 0, Constants::smallArrowSize, Constants::smallArrowSize);
                    toolOpt.rect.moveCenter(menuRect.center());
                    drawPrimitive(PE_IndicatorArrowDown, &toolOpt, p, widget);

                } else if (btn->features & QStyleOptionToolButton::HasMenu) {
                    toolOpt.rect = QRect(0, 0, Constants::toolBtnMenuArrowSize, Constants::toolBtnMenuArrowSize);
                    toolOpt.rect.moveBottomRight(buttonRect.bottomRight() - Constants::toolBtnArrowOffset);
                    drawPrimitive(PE_IndicatorArrowDown, &toolOpt, p, widget);
                }
                return;
            }
            break;
        case CC_GroupBox:
            if (const auto* box = qstyleoption_cast<const QStyleOptionGroupBox*>(opt)) {
                const bool hasCheck = box->subControls & SC_GroupBoxCheckBox;
                const bool hasLabel = box->subControls & SC_GroupBoxLabel;

                QRect checkRect;
                QRect labelRect;
                const QRect frameRect = subControlRect(CC_GroupBox, box, SC_GroupBoxFrame, widget);

                if (hasCheck) {
                    checkRect = subControlRect(CC_GroupBox, box, SC_GroupBoxCheckBox, widget);
                    QStyleOption checkOpt = *box;
                    checkOpt.rect = checkRect;
                    drawPrimitive(PE_IndicatorCheckBox, &checkOpt, p, widget);
                }
                if (hasLabel) {
                    labelRect = subControlRect(CC_GroupBox, box, SC_GroupBoxLabel, widget);
                    p->save();
                    p->setPen(getPen(box->palette, Color::groupBoxText, state));
                    p->drawText(labelRect, (getTextFlags(box) | Qt::AlignCenter), box->text);
                    p->restore();
                }

                if (box->subControls & SC_GroupBoxFrame) {
                    QRect clipRect;
                    if (hasCheck) {
                        clipRect.setTopLeft(checkRect.topLeft() - QPoint(Constants::groupBoxHeaderVerticalPadding, 0));
                    } else if (hasLabel) {
                        clipRect.setTopLeft(labelRect.topLeft() - QPoint(Constants::groupBoxHeaderVerticalPadding, 0));
                    }
                    if (hasLabel) {
                        clipRect.setBottomRight(labelRect.bottomRight() + QPoint(Constants::groupBoxHeaderVerticalPadding, 0));
                    } else if (hasCheck) {
                        clipRect.setBottomRight(checkRect.bottomRight() + QPoint(Constants::groupBoxHeaderVerticalPadding, 0));
                    }

                    QStyleOptionFrame frameOpt;
                    frameOpt.palette = box->palette;
                    frameOpt.features = box->features;
                    frameOpt.state = box->state;
                    frameOpt.rect = frameRect;
                    frameOpt.lineWidth = 1;

                    p->save();
                    if (clipRect.isValid()) {
                        QRegion frameRegion(frameRect);
                        frameRegion -= clipRect;
                        p->setClipRegion(frameRegion);
                    }
                    drawPrimitive(PE_FrameGroupBox, &frameOpt, p, widget);
                    p->restore();
                }
                return;
            }
            break;
        case CC_Dial:
            if (const auto* dial = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                /*
                 * all values are in degrees
                 * 0° is at the 6 o'clock position
                 * when upsideDown is false, value rises in counterclockwise direction
                 *
                 * It doesnt use subControlRect for handle positions, as that would result in duplicate calulations
                 */
                const QRect groove = subControlRect(CC_Dial, dial, SC_DialGroove, widget);

                const int arcLen = dial->dialWrapping ? 360 : Constants::dialRangeNonWaraping; // length in degrees, of the line
                const int startAngle = (360 - arcLen) / 2;
                const qreal dialRadius = (groove.width() - Constants::dialHandleHoverCircleDiameter) / 2;
                const int value = sliderPositionFromValue(dial->minimum, dial->maximum, dial->sliderPosition, arcLen, dial->upsideDown);
                const qreal handleX = qSin(qDegreesToRadians(qreal(startAngle + value))) * dialRadius; // this x and y is for a circle with
                const qreal handleY = qCos(qDegreesToRadians(qreal(startAngle + value))) * dialRadius; // the centre at 0,0 and radius radius

                const QRectF handleHover(groove.left() + dialRadius + handleX,
                                         groove.top() + dialRadius + handleY,
                                         Constants::dialHandleHoverCircleDiameter,
                                         Constants::dialHandleHoverCircleDiameter);

                const QRect slider = groove.adjusted(Constants::dialHandleHoverCircleDiameter / 2, Constants::dialHandleHoverCircleDiameter / 2,
                                                     -Constants::dialHandleHoverCircleDiameter / 2, -Constants::dialHandleHoverCircleDiameter / 2);

                p->save();
                p->setRenderHint(QPainter::Antialiasing);

                p->setBrush(Qt::NoBrush);
                if (dial->upsideDown) {
                    p->setPen(getPen(dial->palette, Color::dialLineAfter, state, 2));
                    p->drawArc(slider, -90 * 16 + startAngle * 16, value * 16);
                    p->setPen(getPen(dial->palette, Color::dialLineBefore, state, 2));
                    p->drawArc(slider, 270 * 16 - startAngle * 16, (value - arcLen) * 16);
                } else {
                    p->setPen(getPen(dial->palette, Color::dialLineBefore, state, 2));
                    p->drawArc(slider, 270 * 16 - startAngle * 16, (value - arcLen) * 16);
                    p->setPen(getPen(dial->palette, Color::dialLineAfter, state, 2));
                    p->drawArc(slider, -90 * 16 + startAngle * 16, value * 16);
                }

                p->setPen(Qt::NoPen);
                p->setBrush(getBrush(dial->palette, Color::dialHandle, state));
                QRectF handle(0, 0, Constants::dialHandleDiameter, Constants::dialHandleDiameter);
                handle.moveCenter(handleHover.center());
                p->drawEllipse(handle);

                if ((state.hovered || state.pressed) && state.enabled) {
                    p->setBrush(getBrush(dial->palette, Color::sliderHandleHoverCircle, state));
                    p->drawEllipse(handleHover);
                }

                p->restore();

                return;
            }
            break;
        default:
            break;
    }
    SuperStyle::drawComplexControl(control, opt, p, widget);
}

void Style::drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const {
    Orchid::State state(opt->state);
    switch (element) {
        case CE_PushButtonBevel:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
                drawPrimitive(PE_PanelButtonCommand, btn, p, widget);

                if (btn->features & QStyleOptionButton::HasMenu) {
                    QStyleOption menuOpt = *btn;
                    menuOpt.rect = QRect(btn->rect.right() - Constants::smallArrowSize - Constants::pushButtonMenuArrowPadding,
                                         btn->rect.top(),
                                         Constants::smallArrowSize,
                                         btn->rect.height());
                    drawPrimitive(PE_IndicatorArrowDown, &menuOpt, p, widget);
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
                        cornerRectSize = qMin(Constants::cornerRadius * 2, // the corner radius is still btnRadius,
                                              qMin(rect.width(),           // it has to be *2 due to implementation
                                                   rect.height() * 2));

                        break;

                    case QTabBar::RoundedEast:
                    case QTabBar::TriangularEast:
                    case QTabBar::RoundedWest:
                    case QTabBar::TriangularWest:
                        cornerRectSize = qMin(Constants::cornerRadius * 2,
                                              qMin(rect.height(),
                                                   rect.width() * 2));
                        break;
                };

                QPainterPath path;
                switch (tab->shape) {
                    case QTabBar::RoundedNorth:
                    case QTabBar::TriangularNorth:
                        path.moveTo(rect.bottomLeft());
                        path.lineTo(rect.left(), rect.top() + Constants::cornerRadius);
                        path.arcTo(QRectF(
                                       rect.topLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.top() + cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.right() - Constants::cornerRadius, rect.top());
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
                        path.lineTo(rect.left(), rect.bottom() - Constants::cornerRadius);
                        path.arcTo(QRectF(
                                       rect.bottomLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.bottom() - cornerRectSize)),
                                   180,
                                   -90);
                        path.lineTo(rect.right() - Constants::cornerRadius, rect.bottom());
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
                        path.lineTo(rect.right() - Constants::cornerRadius, rect.top());
                        path.arcTo(QRectF(
                                       rect.topRight(),
                                       QPointF(rect.right() - cornerRectSize, rect.top() + cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.right(), rect.bottom() - Constants::cornerRadius);
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
                        path.lineTo(rect.left() + Constants::cornerRadius, rect.top());
                        path.arcTo(QRectF(
                                       rect.topLeft(),
                                       QPointF(rect.left() + cornerRectSize, rect.top() + cornerRectSize)),
                                   90,
                                   90);
                        path.lineTo(rect.left(), rect.bottom() - Constants::cornerRadius);
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
                    p->setPen(getPen(tab->palette, Color::tabCheckedOutline, 1));
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

        case CE_ScrollBarSubPage:
        case CE_ScrollBarAddPage:
        case CE_ScrollBarAddLine:
        case CE_ScrollBarSubLine:
            return;

        case CE_ScrollBarSlider: {
            QRect rect;
            const int gapSize = (state.hovered && state.enabled) ? Constants::scrollBarSliderPaddingHover : Constants::scrollBarSliderPadding;
            if (opt->state & QStyle::State_Horizontal) {
                rect = opt->rect.adjusted(0, gapSize + 1, 0, -gapSize); // the +1 is for the separator line above the hovered separator width
            } else {
                rect = opt->rect.adjusted(gapSize + 1, 0, -gapSize, 0);
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

        case CE_MenuItem:
            if (const auto* menu = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
                switch (menu->menuItemType) {
                    case QStyleOptionMenuItem::Normal:
                    case QStyleOptionMenuItem::DefaultItem:
                    case QStyleOptionMenuItem::SubMenu: {
                        state.hovered = menu->state & State_Selected;

                        const QRect contentsRect = menu->rect.adjusted(Constants::menuItemVerticalExternalPadding + Constants::menuItemVerticalInternalPadding,
                                                                       0,
                                                                       -(Constants::menuItemVerticalExternalPadding + Constants::menuItemVerticalInternalPadding),
                                                                       0);

                        // hover rect
                        if (state.hovered && state.enabled) {
                            const QRect hoverRect = menu->rect.adjusted(Constants::menuItemVerticalExternalPadding,
                                                                        0,
                                                                        -(Constants::menuItemVerticalExternalPadding),
                                                                        0);
                            p->save();
                            p->setRenderHints(QPainter::Antialiasing);
                            p->setPen(Qt::NoPen);
                            p->setBrush(getBrush(menu->palette, Color::menuItemHoverBg, state));
                            p->drawRoundedRect(hoverRect, Constants::menuItemBorderRadius, Constants::menuItemBorderRadius);
                            p->restore();
                        }

                        // checks
                        int checkSize = 0;
                        if (menu->menuHasCheckableItems || menu->checkType != QStyleOptionMenuItem::NotCheckable)
                            checkSize = qMin(Constants::checkBoxSize, contentsRect.height());

                        if (menu->checkType != QStyleOptionMenuItem::NotCheckable) {
                            QStyleOption checkOpt(*menu);
                            checkOpt.rect = QRect(contentsRect.left(), contentsRect.top(), checkSize, contentsRect.height());
                            if (menu->checked) {
                                checkOpt.state |= QStyle::State_On;
                                checkOpt.state &= ~QStyle::State_Off;
                            } else {
                                checkOpt.state |= QStyle::State_Off;
                                checkOpt.state &= ~QStyle::State_On;
                            }
                            this->drawPrimitive(menu->checkType == QStyleOptionMenuItem::Exclusive ? QStyle::PE_IndicatorRadioButton :
                                                                                                     QStyle::PE_IndicatorCheckBox,
                                                &checkOpt, p, nullptr);
                        }

                        // icon
                        if (!menu->icon.isNull()) {
                            const QRect iconRect(checkSize > 0 ? contentsRect.left() + checkSize + Constants::menuHorizontalSpacing : contentsRect.left(),
                                                 contentsRect.top(),
                                                 menu->maxIconWidth,
                                                 contentsRect.height());

                            QIcon::Mode iconMode = QIcon::Normal;
                            if (!state.enabled) {
                                iconMode = QIcon::Disabled;
                            } else if (state.hovered) {
                                iconMode = QIcon::Selected;
                            }

                            menu->icon.paint(p, iconRect, Qt::AlignCenter, iconMode);
                        }

                        // label
                        if (!menu->text.isEmpty()) {
                            const int textFlags = getTextFlags(menu);
                            const MenuItemText text = menuItemGetText(menu);

                            p->save();
                            p->setFont(menu->font);
                            if (!text.label.isEmpty()) {
                                const int rightElementSize = menu->menuItemType == QStyleOptionMenuItem::SubMenu ?
                                                                 (text.shortcut.isEmpty() ?
                                                                      qMax(menu->reservedShortcutWidth, Constants::smallArrowSize) :
                                                                      (menu->reservedShortcutWidth + Constants::smallArrowSize + Constants::menuHorizontalSpacing)) :
                                                                 menu->reservedShortcutWidth;
                                int leftElementsSize = (menu->maxIconWidth > 0 ? menu->maxIconWidth + Constants::menuHorizontalSpacing : 0) +
                                                       (checkSize > 0 ? checkSize + Constants::menuHorizontalSpacing : 0);

                                const QRect labelRect(QPoint(contentsRect.left() + leftElementsSize, contentsRect.top()), QPoint(contentsRect.right() - rightElementSize, contentsRect.bottom()));

                                p->setPen(getPen(menu->palette, Color::menuText, state));
                                p->drawText(labelRect, textFlags | Qt::AlignLeft | Qt::AlignVCenter, text.label);
                            }

                            // shortcut
                            if (!text.shortcut.isEmpty()) {
                                QRect shortcutRect(0, contentsRect.top(), menu->reservedShortcutWidth, contentsRect.height());
                                if (menu->menuItemType == QStyleOptionMenuItem::SubMenu) {
                                    shortcutRect.moveRight(contentsRect.right() - Constants::smallArrowSize - Constants::menuHorizontalSpacing);
                                } else {
                                    shortcutRect.moveRight(contentsRect.right());
                                }
                                p->setPen(getPen(menu->palette, Color::menuShortcutText, state));
                                p->drawText(shortcutRect, textFlags | Qt::AlignRight | Qt::AlignVCenter, text.shortcut);
                            }
                            p->restore();
                        }

                        // sub menu arrow
                        if (menu->menuItemType == QStyleOptionMenuItem::SubMenu) {
                            QStyleOption arrowOpt(*menu);
                            arrowOpt.rect = QRect(0, contentsRect.top(), Constants::smallArrowSize, contentsRect.height());
                            arrowOpt.rect.moveRight(contentsRect.right());
                            drawPrimitive(QStyle::PE_IndicatorArrowRight, &arrowOpt, p, widget);
                        }
                        return;
                    }
                    case QStyleOptionMenuItem::Separator: {
                        p->save();
                        const MenuItemText text = menuItemGetText(menu);

                        QRect lineRect;
                        if (text.label.isEmpty()) {
                            lineRect = menu->rect.adjusted(Constants::menuItemVerticalExternalPadding, 0, -Constants::menuItemVerticalExternalPadding, 0);
                        } else {
                            const QSize labelSize = menu->fontMetrics.size((Qt::TextShowMnemonic | Qt::AlignLeft | Qt::AlignVCenter), text.label);
                            const QRect labelRect(menu->rect.left() + Constants::menuItemVerticalExternalPadding, menu->rect.top(), labelSize.width(), menu->rect.height());
                            p->setPen(getPen(menu->palette, Color::menuText));
                            p->setFont(menu->font);
                            p->drawText(labelRect, (getTextFlags(menu) | Qt::AlignLeft | Qt::AlignVCenter), menu->text);

                            lineRect = QRect(QPoint(labelRect.right() + Constants::menuHorizontalSpacing, menu->rect.top()),
                                             QPoint(menu->rect.right() - Constants::menuItemVerticalExternalPadding, menu->rect.bottom()));
                        }

                        p->setPen(getPen(menu->palette, Color::menuSeparator, state));
                        p->drawLine(
                            lineRect.left(),
                            lineRect.center().y(),
                            lineRect.right(),
                            lineRect.center().y());
                        p->restore();
                        return;
                    }

                    default:
                        break;
                }
            }
            break;

        case CE_MenuBarItem:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
                state.hovered = bar->state & State_Selected;

                QRect contentsRect(0, 0,
                                   bar->rect.width() - (Constants::menuBarItemExternalPadding * 2),
                                   bar->rect.height() - (Constants::menuBarItemExternalPadding * 2));
                contentsRect.moveCenter(bar->rect.center());

                int iconWidth = 0;
                if (!bar->icon.isNull()) {
                    iconWidth = qMin(this->pixelMetric(PM_SmallIconSize, bar, widget), contentsRect.height());
                }

                p->save();
                p->setRenderHints(QPainter::Antialiasing);

                p->fillRect(bar->rect, getColor(opt->palette, Color::menuBarBg, state));

                if (state.hovered || state.pressed) {
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(bar->palette, Color::menuBarItemHoverBg, state));
                    p->drawRoundedRect(contentsRect, Constants::menuBarItemBorderRadius, Constants::menuBarItemBorderRadius);
                }

                p->setPen(getPen(bar->palette, Color::menuBarItemText));
                p->setBrush(getBrush(bar->palette, Color::menuBarItemText));

                if (iconWidth > 0) {
                    const QRect iconRect(contentsRect.left() + Constants::menuItemVerticalInternalPadding,
                                         contentsRect.top(),
                                         iconWidth,
                                         contentsRect.height());

                    QIcon::Mode iconMode = QIcon::Normal;
                    if (!state.enabled) {
                        iconMode = QIcon::Disabled;
                    } else if (state.hovered) {
                        iconMode = QIcon::Selected;
                    }

                    bar->icon.paint(p, iconRect, Qt::AlignCenter, iconMode);
                }
                if (!bar->text.isEmpty()) {
                    QRect labelRect(0, contentsRect.top(), 0, contentsRect.height());
                    labelRect.setLeft(
                        contentsRect.left() + Constants::menuItemVerticalInternalPadding + (iconWidth > 0 ? iconWidth + Constants::menuHorizontalSpacing : 0));
                    labelRect.setRight(
                        contentsRect.right() - Constants::menuItemVerticalInternalPadding);
                    p->drawText(labelRect, (Qt::AlignLeft | Qt::AlignVCenter | getTextFlags(bar) | Qt::TextSingleLine), bar->text);
                }
                p->restore();
                return;
            }
            break;
        case CE_MenuBarEmptyArea: {
            p->save();
            p->fillRect(opt->rect, getColor(opt->palette, Color::menuBarBg, state));
            p->setPen(getPen(opt->palette, Color::line, state, 1));
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());
            p->restore();
            return;
        }

        case CE_ComboBoxLabel:
            if (const auto* combo = qstyleoption_cast<const QStyleOptionComboBox*>(opt)) {
                const QRect labelRect = combo->rect.adjusted(Constants::comboTextLeftPadding, 0, -Constants::comboArrowWidth, 0);
                const bool hasIcon = !combo->currentIcon.isNull() && combo->iconSize.isValid();
                if (hasIcon) {
                    QRect iconRect(QPoint(0, 0), combo->iconSize);
                    iconRect.moveCenter(labelRect.center());
                    iconRect.moveLeft(labelRect.left());
                    combo->currentIcon.paint(p,
                                             iconRect,
                                             Qt::AlignCenter,
                                             state.enabled ? QIcon::Normal : QIcon::Disabled);
                }

                if (!combo->currentText.isEmpty() && !combo->editable) {
                    QRect textRect(labelRect);
                    textRect.setLeft(labelRect.left() + (hasIcon ? combo->iconSize.width() + Constants::lineEditTextVerticalPadding : 0));

                    p->save();
                    p->setPen(getPen(combo->palette, Orchid::comboBoxUneditableText));
                    p->setBrush(Qt::NoBrush);
                    p->drawText(textRect, (getTextFlags(combo) | Qt::AlignLeft | Qt::AlignVCenter), combo->currentText);
                    p->restore();
                }
                return;
            }
            break;

        case CE_ToolButtonLabel:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
                QRect rect = btn->rect.adjusted(Constants::toolBtnLabelVerticalPadding,
                                                Constants::toolBtnLabelHorizontalPadding,
                                                -Constants::toolBtnLabelVerticalPadding,
                                                -Constants::toolBtnLabelHorizontalPadding);

                QRect iconRect;
                QRect textRect;
                Qt::Alignment textHalign = Qt::AlignLeft;
                switch (btn->toolButtonStyle) {
                    case Qt::ToolButtonIconOnly:
                        iconRect = QRect(QPoint(0, 0), btn->iconSize);
                        iconRect.moveCenter(rect.center());
                        break;
                    case Qt::ToolButtonTextOnly:
                        textRect = rect;
                        break;
                    case Qt::ToolButtonTextBesideIcon:
                    case Qt::ToolButtonFollowStyle:
                        iconRect = QRect(rect.left(), rect.top(), btn->iconSize.width(), rect.height());
                        textRect = QRect(QPoint(iconRect.right() + Constants::toolbtnLabelSpacing, rect.top()), rect.bottomRight());
                        break;
                    case Qt::ToolButtonTextUnderIcon:
                        iconRect = QRect(rect.left(), rect.top(), rect.width(), btn->iconSize.height());
                        textRect = QRect(QPoint(rect.left(), iconRect.bottom() + Constants::toolbtnLabelSpacing), rect.bottomRight());
                        textHalign = Qt::AlignHCenter;
                        break;
                }
                p->save();
                p->setPen(getPen(btn->palette, Color::toolBtnText, state));
                p->setFont(btn->font);

                if (iconRect.isValid()) {
                    if (!btn->icon.isNull()) {
                        btn->icon.paint(p, iconRect, Qt::AlignCenter, state.enabled ? QIcon::Normal : QIcon::Disabled);
                    } else if (btn->arrowType != Qt::NoArrow) {
                        PrimitiveElement arrow;
                        switch (btn->arrowType) {
                            case Qt::RightArrow:
                                arrow = PE_IndicatorArrowRight;
                                break;
                            case Qt::LeftArrow:
                                arrow = PE_IndicatorArrowLeft;
                                break;
                            case Qt::UpArrow:
                                arrow = PE_IndicatorArrowUp;
                                break;
                            case Qt::DownArrow:
                            case Qt::NoArrow:
                                arrow = PE_IndicatorArrowDown;
                                break;
                        }
                        QStyleOption arrowOpt(*btn);
                        arrowOpt.rect = iconRect;
                        drawPrimitive(arrow, &arrowOpt, p);
                    }
                }
                if (textRect.isValid() && !btn->text.isEmpty()) {
                    p->drawText(textRect, (getTextFlags(btn) | textHalign | Qt::AlignVCenter), btn->text);
                }
                p->restore();

                return;
            }
            break;
        case CE_ToolBar:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionToolBar*>(opt)) {
                QLine separatorLine;
                if (bar->toolBarArea & Qt::TopToolBarArea) {
                    separatorLine.setP1(bar->rect.bottomLeft());
                    separatorLine.setP2(bar->rect.bottomRight());
                } else if (bar->toolBarArea & Qt::BottomToolBarArea) {
                    separatorLine.setP1(bar->rect.topLeft());
                    separatorLine.setP2(bar->rect.topRight());
                } else if (bar->toolBarArea & Qt::LeftToolBarArea) {
                    separatorLine.setP1(bar->rect.topRight());
                    separatorLine.setP2(bar->rect.bottomRight());
                } else if (bar->toolBarArea & Qt::RightToolBarArea) {
                    separatorLine.setP1(bar->rect.topLeft());
                    separatorLine.setP2(bar->rect.bottomLeft());
                }

                p->save();
                p->setBrush(Qt::NoBrush);
                p->fillRect(bar->rect, getColor(bar->palette, Color::toolBarBg, state));
                p->setPen(getPen(bar->palette, Color::line, state, 1));
                p->drawLine(separatorLine);
                p->restore();
                return;
            }
            break;

        case CE_ProgressBar:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                QStyleOptionProgressBar barOpt = *bar;

                barOpt.rect = subElementRect(SE_ProgressBarContents, bar, widget);
                drawControl(CE_ProgressBarContents, &barOpt, p, widget);

                if (bar->textVisible) {
                    barOpt.rect = subElementRect(SE_ProgressBarLabel, bar, widget);
                    drawControl(CE_ProgressBarLabel, &barOpt, p, widget);
                }
                return;
            }
            break;
        case CE_ProgressBarLabel:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                const bool horizontal = bar->state & State_Horizontal;

                p->save();
                p->setRenderHints(QPainter::Antialiasing);
                p->setPen(getPen(bar->palette, Color::progressBarText, state));
                QRect textRect;
                if (horizontal) {
                    textRect = bar->rect;
                } else {
                    p->translate(bar->rect.topLeft());
                    p->rotate(bar->bottomToTop ? -90 : 90);

                    textRect = QRect(QPoint(bar->bottomToTop ? -bar->rect.height() : 0,
                                            bar->bottomToTop ? 0 : -bar->rect.width()),
                                     bar->rect.size().transposed());
                }
                p->drawText(textRect, (getTextFlags(bar) | Qt::AlignCenter), bar->text);
                p->restore();
                return;
            }
            break;

        case CE_ProgressBarContents:
            // TODO: make the busy indicator animated
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                const bool horizontal = bar->state & State_Horizontal;
                p->save();
                p->setRenderHints(QPainter::Antialiasing);
                p->setPen(Qt::NoPen);

                // groove
                p->setBrush(getBrush(bar->palette, Color::progressBarIndicatorBg, state));
                if (horizontal) {
                    p->drawRoundedRect(bar->rect, bar->rect.height() / 2, bar->rect.height() / 2);
                } else {
                    p->drawRoundedRect(bar->rect, bar->rect.width() / 2, bar->rect.width() / 2);
                }
                if (bar->progress <= 0 && bar->maximum > 0) {
                    p->restore();
                    return;
                }

                // busy indicator
                if (bar->maximum == 0 && bar->minimum == 0) {
                    p->setBrush(getBrush(bar->palette, Color::progressBarIndicator, state));
                    const int dashLen = (horizontal ? bar->rect.height() : bar->rect.width()) * 2;

                    if (horizontal) {
                        for (int position = bar->rect.left(); position <= (bar->rect.right() - dashLen); position += (dashLen * 2)) {
                            p->drawRoundedRect(position, bar->rect.top(), dashLen, bar->rect.height(),
                                               bar->rect.height() / 2.0, bar->rect.height() / 2.0);
                        }
                    } else {
                        for (int position = bar->rect.top(); position <= (bar->rect.bottom() - dashLen); position += (dashLen * 2)) {
                            p->drawRoundedRect(bar->rect.left(), position, bar->rect.width(), dashLen,
                                               bar->rect.width() / 2.0, bar->rect.width() / 2.0);
                        }
                    }

                    p->restore();
                    return;
                }

                // progress indicator
                const int progressLen = sliderPositionFromValue(bar->minimum,
                                                                bar->maximum,
                                                                bar->progress,
                                                                horizontal ? bar->rect.width() : bar->rect.height(),
                                                                bar->invertedAppearance);

                QRect progressRect;
                if (horizontal) {
                    if (bar->invertedAppearance) {
                        progressRect = QRect(0, bar->rect.top(), bar->rect.width() - progressLen, bar->rect.height());
                        progressRect.moveRight(bar->rect.right());
                    } else {
                        progressRect = QRect(bar->rect.left(), bar->rect.top(), progressLen, bar->rect.height());
                    }
                } else {
                    if (bar->invertedAppearance) {
                        progressRect = QRect(bar->rect.left(), 0, bar->rect.width(), bar->rect.height() - progressLen);
                        progressRect.moveBottom(bar->rect.bottom());
                    } else {
                        progressRect = QRect(bar->rect.left(), bar->rect.top(), bar->rect.width(), progressLen);
                    }
                }

                p->setBrush(getBrush(bar->palette, Color::progressBarIndicator, state));
                if (horizontal) {
                    p->drawRoundedRect(progressRect, bar->rect.height() / 2, bar->rect.height() / 2);
                } else {
                    p->drawRoundedRect(progressRect, bar->rect.width() / 2, bar->rect.width() / 2);
                }
                p->restore();
                return;
            }
            break;

        case CE_HeaderSection:
            if (const auto* header = qstyleoption_cast<const QStyleOptionHeader*>(opt)) {
                p->save();
                p->fillRect(header->rect, getColor(header->palette, Color::viewHeaderBg, state));

                p->setPen(getPen(header->palette, Color::line, 1));
                if (header->orientation == Qt::Horizontal) {
                    if (header->position != QStyleOptionHeader::OnlyOneSection && header->position != QStyleOptionHeader::End) {
                        p->drawLine(header->rect.topRight(), header->rect.bottomRight());
                    }
                    p->drawLine(header->rect.bottomLeft(), header->rect.bottomRight());
                } else {
                    if (header->position != QStyleOptionHeader::OnlyOneSection && header->position != QStyleOptionHeader::End) {
                        p->drawLine(header->rect.bottomLeft(), header->rect.bottomRight());
                    }
                    p->drawLine(header->rect.topRight(), header->rect.bottomRight());
                }
                p->restore();
                return;
            }
            break;

        case CE_HeaderEmptyArea: {
            p->save();
            p->fillRect(opt->rect, getColor(opt->palette, Color::viewHeaderEmptyAreaBg, state));
            p->setPen(getPen(opt->palette, Color::line, state, 1));
            if (opt->state & State_Horizontal) {
                p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());
            } else {
                p->drawLine(opt->rect.topRight(), opt->rect.bottomRight());
            }
            p->restore();
            return;
        }

        case CE_SizeGrip:
            return;

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
                    p->setBrush(getBrush(btn->palette, Color::buttonBg, state));
                }

                p->drawRoundedRect(btn->rect, Constants::cornerRadius, Constants::cornerRadius);
                p->restore();
                return;
            }
            break;
        case PE_FrameFocusRect:
            // this also has QStyleOptionFocusRect - in case of bugs look here
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setBrush(Qt::NoBrush);
            p->setPen(getPen(opt->palette, Color::focusRect, 2));
            p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), Constants::cornerRadius, Constants::cornerRadius);
            p->restore();
            return;

        case PE_IndicatorCheckBox:
        case PE_IndicatorRadioButton: {
            const int indicatorSize = qMin(Constants::checkBoxSize, qMin(opt->rect.height() - 1, opt->rect.width()) - 1);
            QRect indicatorRect = QRect(0, 0, indicatorSize, indicatorSize);
            indicatorRect.moveCenter(opt->rect.center());

            p->save();
            p->setRenderHints(QPainter::Antialiasing);

            if (state.hovered && state.enabled) {
                const int size = qMin(Constants::checkBoxHoverCircleSize, qMin(opt->rect.height(), opt->rect.width()));
                QRect hoverRect(0, 0, size, size);
                hoverRect.moveCenter(opt->rect.center());

                p->setPen(Qt::NoPen);
                p->setBrush(getBrush(opt->palette,
                                     (opt->state & State_Off) ? Color::checkBoxHoverCircle : Color::checkBoxHoverCircleChecked,
                                     state));
                p->drawEllipse(hoverRect);
            }

            if (opt->state & (QStyle::State_Off)) {
                p->setPen(getPen(opt->palette, Color::checkBoxOutline, state, 2));
                p->setBrush(Qt::NoBrush);
                p->drawEllipse(indicatorRect.adjusted(1, 1, -1, -1));
                p->restore();
                return;
            }

            p->setPen(Qt::NoPen);
            p->setBrush(getBrush(opt->palette, Color::checkBoxInside, state));
            p->drawEllipse(indicatorRect);

            if (element == QStyle::PE_IndicatorCheckBox) {
                p->setPen(getPen(opt->palette, Color::checkBoxCheck, state, 2));
                p->setBrush(Qt::NoBrush);

                const double widthOffset = opt->rect.width() - indicatorSize;

                if (opt->state & QStyle::State_NoChange) {
                    const double adjustment = indicatorSize / 4.0f;
                    const double y = opt->rect.center().y() + 1;

                    p->drawLine(QLineF(opt->rect.left() + widthOffset + adjustment + 1, y, opt->rect.right() - widthOffset - adjustment, y));

                } else {
                    const auto center = opt->rect.center();
                    const QPointF points[3] = {
                        QPointF(center.x() - indicatorSize / 4.0, center.y()),
                        QPointF(center.x() - indicatorSize / 10.0, center.y() + indicatorSize / 5.0),
                        QPointF(center.x() + indicatorSize / 4.0, center.y() - indicatorSize / 7.5),
                    };

                    p->drawPolyline(points, 3);
                }
            } else {
                const double adjustment = indicatorSize * 0.35;

                p->setPen(Qt::NoPen);
                p->setBrush(getBrush(opt->palette, Color::checkBoxCheck, state));
                p->drawEllipse(indicatorRect.toRectF().adjusted(adjustment, adjustment, -adjustment, -adjustment));
            }

            p->restore();
            return;
        }
        case PE_FrameTabWidget:
            p->save();
            p->setBrush(getBrush(opt->palette, Color::tabWidgetPageArea));
            p->setPen(getPen(opt->palette, Color::tabCheckedOutline, 1));
            p->drawRect(opt->rect.adjusted(1, 1, -1, -1));
            p->restore();
            return;

        case PE_FrameTabBarBase:
            p->save();
            p->setBrush(Qt::NoBrush);
            p->setPen(getPen(opt->palette, Color::tabCheckedOutline, 1));
            p->drawRect(opt->rect);
            p->restore();
            return;

        case PE_IndicatorTabClose: {
            const int indicatorSize = qMin(Constants::tabCloseIndicatorSize, qMin(opt->rect.width(), opt->rect.height()));
            QRectF indicatorRect(0, 0, indicatorSize, indicatorSize);
            indicatorRect.moveCenter(opt->rect.toRectF().center());

            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(QPen(getBrush(opt->palette, Color::tabCloseIndicator, state), 2, Qt::SolidLine, Qt::RoundCap));

            p->drawLine(indicatorRect.topLeft(), indicatorRect.bottomRight());
            p->drawLine(indicatorRect.bottomLeft(), indicatorRect.topRight());

            if ((state.hovered || state.pressed) && state.enabled) {
                p->setBrush(getBrush(opt->palette, Color::tabCloseIndicatorHoverCircle, state));
                p->setPen(Qt::NoPen);
                p->drawEllipse(opt->rect);
            }
            p->restore();
            return;
        }

        case PE_PanelLineEdit:
            if (const QStyleOptionFrame* edit = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
                if (edit->lineWidth > 0) {
                    p->save();
                    p->setRenderHints(QPainter::Antialiasing);
                    p->setPen(Qt::NoPen);
                    p->setBrush(getBrush(edit->palette, Color::lineEditBg, state));
                    p->drawRoundedRect(opt->rect, Constants::cornerRadius, Constants::cornerRadius);
                    p->restore();
                    this->drawPrimitive(PE_FrameLineEdit, edit, p, widget);
                }
                return;
            }
            break;

        case PE_FrameLineEdit:
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(getPen(opt->palette, Color::lineEditOutline, state, 2));
            p->setBrush(Qt::NoBrush);
            p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), Constants::cornerRadius, Constants::cornerRadius);
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
                    Constants::spinIndicatorIconSize);
                QRect indicatorRect(0, 0, indicatorSize, indicatorSize);
                indicatorRect.moveCenter(spin->rect.center());

                p->save();
                p->setBrush(Qt::NoBrush);
                p->setPen(getPen(spin->palette, Color::spinBoxIndicator, state, 2));
                p->drawLine(indicatorRect.left() + 1, indicatorRect.center().y() + 1, indicatorRect.right(), indicatorRect.center().y() + 1);
                if (element == QStyle::PE_IndicatorSpinUp || element == QStyle::PE_IndicatorSpinPlus)
                    p->drawLine(indicatorRect.center().x() + 1, indicatorRect.top() + 1, indicatorRect.center().x() + 1, indicatorRect.bottom());
                p->restore();
                return;
            }
            break;
        case PE_PanelMenu: {
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            p->setBrush(getBrush(opt->palette, Color::menuBg, state));
            p->drawRoundedRect(opt->rect.adjusted(Constants::menuTransparentPadding,
                                                  Constants::menuTransparentPadding,
                                                  -Constants::menuTransparentPadding,
                                                  -Constants::menuTransparentPadding),
                               Constants::menuBorderRadius,
                               Constants::menuBorderRadius);
            p->restore();
            return;
        }
        case PE_PanelButtonTool: {
            // this also gets called from titlebar, so i cant have everithing in QStyleOptionToolButton
            bool justIcon = true;
            if (const auto* btn = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
                justIcon = (btn->toolButtonStyle == Qt::ToolButtonIconOnly);
            }

            Color color = toolBtnBg;
            if ((opt->state & State_AutoRaise) && (opt->state & State_On)) {
                color = toolBtnBgAutoRiseChecked;
            } else if (opt->state & (State_AutoRaise)) {
                color = toolBtnBgAutoRise;
            } else if (opt->state & State_On) {
                color = toolBtnBgChecked;
            }
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            if (state.hasFocus) {
                p->setPen(getPen(opt->palette, Color::toolBtnFocusOutline, 2));
            } else {
                p->setPen(Qt::NoPen);
            }
            p->setBrush(getBrush(opt->palette, color, state));
            if ((opt->rect.width() == opt->rect.height()) && justIcon) {
                p->drawEllipse(opt->rect.adjusted(1, 1, -1, -1));
            } else {
                const qreal cornerRadius = qMin(qreal(Constants::cornerRadius), opt->rect.height() / 2.0);
                p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), cornerRadius, cornerRadius);
            }
            p->restore();
            return;
        }

        case PE_IndicatorButtonDropDown: {
            if ((opt->state & State_AutoRaise) && ((!state.hovered && !state.pressed) || !state.enabled))
                return;

            const int padding = qMin(int(opt->rect.height() / 4.0), Constants::toolBtnMenuSeparatorHorizontalPadding);
            p->save();
            p->setPen(getPen(opt->palette, Color::toolBtnMenuSeparator, 1));
            p->drawLine(opt->rect.topLeft() + QPoint(0, padding),
                        opt->rect.bottomLeft() - QPoint(0, padding));
            p->restore();
            return;
        }

        case PE_IndicatorToolBarHandle:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionToolBar*>(opt)) {
                if (!(bar->features & QStyleOptionToolBar::Movable)) {
                    return;
                }

                p->save();
                p->setPen(getPen(bar->palette, Color::toolBarHandle, 1));
                if (bar->state & State_Horizontal) {
                    // 2 is 2*1 for the line widths
                    QRect linesRect = QRect(0, 0,
                                            qMin(bar->rect.width(), 2 + Constants::toolBarHandleLineSpacing),
                                            bar->rect.height() - qMin(Constants::toolBarHandleHorizontalPadding * 2, bar->rect.height() / 2));
                    linesRect.moveCenter(bar->rect.center());
                    p->drawLine(linesRect.topLeft(), linesRect.bottomLeft());
                    p->drawLine(linesRect.topRight(), linesRect.bottomRight());
                } else {
                    // 2 is 2*1 for the line widths
                    QRect linesRect = QRect(0, 0,
                                            bar->rect.width() - qMin(Constants::toolBarHandleHorizontalPadding * 2, bar->rect.width() / 2),
                                            qMin(bar->rect.height(), 2 + Constants::toolBarHandleLineSpacing));
                    linesRect.moveCenter(bar->rect.center());
                    p->drawLine(linesRect.topLeft(), linesRect.topRight());
                    p->drawLine(linesRect.bottomLeft(), linesRect.bottomRight());
                }
                p->restore();
                return;
            }
            break;
        case PE_IndicatorToolBarSeparator: {
            p->save();
            p->setPen(getPen(opt->palette, Color::toolBarSeparator, 1));
            if (opt->state & State_Horizontal) {
                // padding for the longer side (top and bottom, when toolbar is horizontal)
                const int padding = qMin(Constants::toolBarSeparatorHorizontalPadding * 2, opt->rect.height() / 2);
                p->drawLine(opt->rect.center().x(),
                            opt->rect.top() + padding,
                            opt->rect.center().x(),
                            opt->rect.bottom() - padding);
            } else {
                const int padding = qMin(Constants::toolBarSeparatorHorizontalPadding * 2, opt->rect.width() / 2);
                p->drawLine(opt->rect.left() + padding,
                            opt->rect.center().y(),
                            opt->rect.right() - padding,
                            opt->rect.center().y());
            }
            p->restore();
        }

        case PE_IndicatorBranch: {
            p->save();
            p->setPen(getPen(opt->palette, Color::branchIndicator, state, 1));

            if (opt->state & State_Children) {
                const int arrowSize = qMin(Constants::treeIndicatorArrowSize, qMin(opt->rect.width(), opt->rect.height()));
                QStyleOption arrowOpt = *opt;
                arrowOpt.rect = QRect(0, 0, arrowSize, arrowSize);
                arrowOpt.rect.moveCenter(opt->rect.center());
                arrowOpt.state.setFlag(State_Enabled, true);

                if (arrowOpt.rect.top() - Constants::treeIndicatorArrowLinePadding > opt->rect.top()) {
                    p->drawLine(opt->rect.center().x(), opt->rect.top(), opt->rect.center().x(), arrowOpt.rect.top() - Constants::treeIndicatorArrowLinePadding);
                }
                if ((opt->state & State_Sibling) && (arrowOpt.rect.bottom() + Constants::treeIndicatorArrowLinePadding < opt->rect.bottom())) {
                    p->drawLine(opt->rect.center().x(), arrowOpt.rect.bottom() + Constants::treeIndicatorArrowLinePadding, opt->rect.center().x(), opt->rect.bottom());
                }

                if (opt->state & State_Open) {
                    this->drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, p, widget);
                } else {
                    this->drawPrimitive(PE_IndicatorArrowRight, &arrowOpt, p, widget);
                }
                p->restore();
                return;
            }

            if (opt->state & State_Sibling) {
                p->drawLine(opt->rect.center().x(), opt->rect.top(), opt->rect.center().x(), opt->rect.bottom());
                if (opt->state & State_Item) {
                    p->drawLine(opt->rect.center(), QPoint(opt->rect.right(), opt->rect.center().y()));
                }
                p->restore();
                return;
            }

            if (opt->state & State_Item) {
                const int curveSize = qMin(Constants::treeIndicatorLastCornerRadius * 2, int(qMin(opt->rect.width(), opt->rect.height())));
                QRect curveRect(0, 0, curveSize, curveSize);
                curveRect.moveBottomLeft(opt->rect.center());

                p->drawLine(opt->rect.center().x(), opt->rect.top(), curveRect.left(), curveRect.center().y());
                p->drawLine(curveRect.center().x(), curveRect.top() + curveRect.height(), opt->rect.right(), curveRect.top() + curveRect.height());
                p->setRenderHints(QPainter::Antialiasing);
                p->drawArc(curveRect, 180 * 16, 90 * 16);

                p->restore();
                return;
            }

            p->restore();
            return;
        }

        case PE_FrameGroupBox:
            if (const auto* frame = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
                p->save();
                p->setRenderHints(QPainter::Antialiasing);
                p->setBrush(Qt::NoBrush);
                p->setPen(getPen(frame->palette, Color::groupBoxLine, state, frame->lineWidth));

                if (frame->features & QStyleOptionFrame::Flat) {
                    p->drawLine(frame->rect.toRectF().topLeft() + QPointF(.5, .5),
                                frame->rect.toRectF().topRight() + QPointF(-.5, .5));
                } else {
                    p->drawRoundedRect(frame->rect.toRectF().adjusted(.5, .5, -.5, -.5), Constants::cornerRadius, Constants::cornerRadius);
                }
                p->restore();
                return;
            }
            break;

        case PE_IndicatorHeaderArrow:
            if (const auto* header = qstyleoption_cast<const QStyleOptionHeader*>(opt)) {
                switch (header->sortIndicator) {
                    case QStyleOptionHeader::SortUp:
                        drawPrimitive(PE_IndicatorArrowUp, header, p, widget);
                        return;
                    case QStyleOptionHeader::SortDown:
                        drawPrimitive(PE_IndicatorArrowDown, header, p, widget);
                        return;
                    case QStyleOptionHeader::None:
                        return;
                }
            }
            break;

        case PE_FrameButtonTool:
            return;

        case PE_FrameMenu:
            return;

        case PE_IndicatorArrowUp:
        case PE_IndicatorArrowDown:
        case PE_IndicatorArrowLeft:
        case PE_IndicatorArrowRight: {
            const int size = qMin(opt->rect.width(), opt->rect.height());
            const int tipOffset = size % 2;
            /* tipOffset: is so for odd size rect the arrow is is still symetrical
             * e.g for the down arrow:
             * if the width of the rect is odd the tip's x would not be a whole number,
             * so instead of a recangle I draw a trapezoid with the tip being 2px long
             * because trapezoid has 4 sides, in p->drawPolygon I do 4 if trapezoid and 3
             * if triangle.
             */

            int width = 0;
            int height = 0;
            if (element == PE_IndicatorArrowUp || element == PE_IndicatorArrowDown) {
                width = size;
                height = size / 2;
            } else {
                height = size;
                width = size / 2;
            }

            QPoint points[4];
            switch (element) {
                case PE_IndicatorArrowUp:
                    points[0] = QPoint(0, height);
                    points[1] = QPoint(width, height);
                    points[2] = QPoint(width / 2, 0);
                    points[3] = QPoint(width / 2 + tipOffset, 0);
                    break;
                case PE_IndicatorArrowDown:
                    points[0] = QPoint(0, 0);
                    points[1] = QPoint(width, 0);
                    points[2] = QPoint(width / 2, height);
                    points[3] = QPoint(width / 2 + tipOffset, height);
                    break;
                case PE_IndicatorArrowRight:
                    points[0] = QPoint(0, 0);
                    points[1] = QPoint(0, height);
                    points[2] = QPoint(width, height / 2);
                    points[3] = QPoint(width, height / 2 + tipOffset);
                    break;
                case PE_IndicatorArrowLeft:
                    points[0] = QPoint(width, 0);
                    points[1] = QPoint(width, height);
                    points[2] = QPoint(0, height / 2);
                    points[3] = QPoint(0, height / 2 + tipOffset);
                    break;
                default:
                    break;
            }
            p->save();
            p->setRenderHints(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            p->setBrush(getBrush(opt->palette, Color::indicatorArrow, state));
            p->translate(opt->rect.left() + (opt->rect.width() - width) / 2,
                         opt->rect.top() + (opt->rect.height() - height) / 2);
            p->drawPolygon(points, 3 + tipOffset);
            p->restore();
            return;
        }

        default:
            break;
    }
    SuperStyle::drawPrimitive(element, opt, p, widget);
}
void Style::polish(QWidget* widget) {
    SuperStyle::polish(widget);
    if (widget->inherits("QAbstractButton") ||
        widget->inherits("QTabBar") ||
        widget->inherits("QScrollBar") ||
        widget->inherits("QAbstractSlider") ||
        widget->inherits("QAbstractSpinBox") ||
        widget->inherits("QComboBox") ||
        widget->inherits("QLineEdit")) {
        widget->setAttribute(Qt::WA_Hover, true);
    }
    if (widget->inherits("QScrollBar")) {
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }
    if (QMenu* menu = qobject_cast<QMenu*>(widget)) {
        menu->setAttribute(Qt::WA_TranslucentBackground);
        if (menu->graphicsEffect() == nullptr) {
            QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(menu);
            shadow->setColor(QColor(10, 10, 10, 100));
            shadow->setOffset(0, 1);
            shadow->setBlurRadius(Constants::menuShadowSize);
            menu->setGraphicsEffect(shadow);
        }
    }
}

void Style::unpolish(QWidget* widget) {
    SuperStyle::unpolish(widget);
    if (widget->inherits("QAbstractButton") ||
        widget->inherits("QTabBar") ||
        widget->inherits("QScrollBar") ||
        widget->inherits("QAbstractSlider") ||
        widget->inherits("QAbstractSpinBox") ||
        widget->inherits("QComboBox") ||
        widget->inherits("QLineEdit")) {
        widget->setAttribute(Qt::WA_Hover, false);
    }
    if (widget->inherits("QScrollBar")) {
        widget->setAttribute(Qt::WA_OpaquePaintEvent, true);
    }
    if (QMenu* menu = qobject_cast<QMenu*>(widget)) {
        menu->setAttribute(Qt::WA_TranslucentBackground, false);
        menu->setGraphicsEffect(nullptr);
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
        case PM_TabCloseIndicatorWidth:
        case PM_TabCloseIndicatorHeight:
            return 24;
        case PM_SliderThickness:
        case PM_SliderControlThickness:
        case PM_SliderLength:
            return Constants::sliderHandleHoverCircleDiameter;
        case PM_SliderTickmarkOffset:
            return 3;
        case PM_ScrollBarSliderMin:
            return 40;
        case PM_MenuHMargin:
            return 0 + Constants::menuTransparentPadding;
        case PM_MenuVMargin:
            return 5 + Constants::menuTransparentPadding;
        case PM_SubMenuOverlap:
            return 0;
        case PM_MenuBarItemSpacing:
            return 0;
        case PM_MenuBarHMargin:
            return 2;
        case PM_MenuBarVMargin:
            return 2;
        case PM_MenuBarPanelWidth:
            return 0;
        case PM_ScrollView_ScrollBarOverlap:
            return Constants::scrollBarThickness;
        case PM_ToolBarFrameWidth:
            return 0;
        case PM_ToolBarItemMargin:
        case PM_ToolBarItemSpacing:
            return 3;
        case PM_ToolBarSeparatorExtent:
            return 3;
        case PM_ToolBarHandleExtent:
            return (Constants::toolBarHandleVerticalPadding * 2) + 2 + Constants::toolBarHandleLineSpacing; // 2 is for the line thickness
        case PM_ProgressBarChunkWidth:
            return 2;
        default:
            break;
    }
    return SuperStyle::pixelMetric(m, opt, widget);
}

int Style::styleHint(QStyle::StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const {
    switch (hint) {
        case SH_UnderlineShortcut:
            return false;
        case SH_MenuBar_MouseTracking:
            return true;
        case SH_Menu_Scrollable:
            return true;
        case SH_Menu_MouseTracking:
            return true;
        case SH_Menu_SubMenuPopupDelay:
            return 150;
        case SH_Menu_SloppySubMenus:
            return true;
        case SH_Menu_SupportsSections:
            return true;
        case SH_DrawMenuBarSeparator:
            return false;
        case SH_ComboBox_Popup:
            return false;
        case SH_ComboBox_PopupFrameStyle:
            return 0;
        case SH_ScrollBar_LeftClickAbsolutePosition:
            return true;
        case SH_ToolButtonStyle:
            return Qt::ToolButtonTextBesideIcon;
        default:
            break;
    }
    return SuperStyle::styleHint(hint, option, widget, returnData);
}

QRect Style::subElementRect(QStyle::SubElement element, const QStyleOption* opt, const QWidget* widget) const {
    switch (element) {
        case SE_PushButtonFocusRect:
            return opt->rect;
            break;
        case SE_RadioButtonIndicator:
        case SE_CheckBoxIndicator: {
            const int size = qMin(Constants::checkBoxHoverCircleSize, qMin(opt->rect.width(), opt->rect.height()));
            QRect rect(0, 0, size, size);
            rect.moveTopLeft(opt->rect.topLeft());
            return rect;
        }
        case SE_LineEditContents:
            if (const QStyleOptionFrame* edit = qstyleoption_cast<const QStyleOptionFrame*>(opt)) {
                if (edit->lineWidth <= 0) {
                    return edit->rect;
                }
                return edit->rect.adjusted(Constants::lineEditTextVerticalPadding,
                                           edit->lineWidth,
                                           -Constants::lineEditTextVerticalPadding,
                                           -edit->lineWidth);
            }
            break;
        case SE_ToolBarHandle:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionToolBar*>(opt)) {
                if (!(bar->features & QStyleOptionToolBar::Movable)) {
                    return QRect();
                }
                const int thickness = pixelMetric(PM_ToolBarHandleExtent);
                if (opt->state & State_Horizontal) {
                    return QRect(opt->rect.left(),
                                 opt->rect.top(),
                                 qMin(opt->rect.width(), thickness),
                                 opt->rect.height());
                }
                return QRect(opt->rect.left(),
                             opt->rect.top(),
                             opt->rect.width(),
                             qMin(opt->rect.height(), thickness));
            }
            break;
        case SE_ProgressBarContents:
        case SE_ProgressBarGroove:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                const bool horizontal = bar->state & State_Horizontal;
                if (!bar->textVisible) {
                    QRect contentsRect;
                    if (horizontal) {
                        contentsRect = QRect(0, 0, bar->rect.width(), Constants::progressBarThickness);
                    } else {
                        contentsRect = QRect(0, 0, Constants::progressBarThickness, bar->rect.height());
                    }
                    contentsRect.moveCenter(bar->rect.center());
                    return contentsRect;
                }
                if (horizontal) {
                    const int labelRectWidth = this->subElementRect(SE_ProgressBarLabel, bar, widget).width();
                    return QRect(bar->rect.left(),
                                 (bar->rect.top() + bar->rect.height() / 2) - Constants::progressBarThickness / 2,
                                 bar->rect.width() - labelRectWidth,
                                 Constants::progressBarThickness);
                }
                const int labelRectHeight = this->subElementRect(SE_ProgressBarLabel, bar, widget).height();
                return QRect((bar->rect.left() + bar->rect.width() / 2) - Constants::progressBarThickness / 2,
                             bar->rect.top(),
                             Constants::progressBarThickness,
                             bar->rect.height() - labelRectHeight);
            }
            break;
        case SE_ProgressBarLabel:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                const bool horizontal = bar->state & State_Horizontal;

                if (!bar->textVisible || bar->minimum == bar->maximum) {
                    return QRect();
                }

                QSize textSizeActual = bar->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), bar->text);
                QSize textSizeDefault = bar->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), "100%");
                QSize& textSize = textSizeDefault.width() > textSizeActual.width() ? textSizeDefault : textSizeActual;

                if (!horizontal) {
                    textSize.transpose();
                }
                QRect rect;
                if (horizontal) {
                    rect = QRect(0, bar->rect.top(), qMin(textSize.width() + Constants::progressBarLabelVerticalPadding * 2, bar->rect.width()), bar->rect.height());
                    rect.moveRight(bar->rect.right());
                } else {
                    rect = QRect(bar->rect.left(), 0, bar->rect.width(), qMin(textSize.height() + Constants::progressBarLabelVerticalPadding * 2, bar->rect.height()));
                    rect.moveBottom(bar->rect.bottom());
                }
                return rect;
            }
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
                        const int width = qMin(spin->rect.width() / 3, Constants::spinIndicatorWidth);
                        QRect rect(spin->rect);
                        rect.setWidth(width);
                        rect.moveRight(spin->rect.right());
                        return rect;
                    }
                    case SC_SpinBoxDown: {
                        const int width = qMin(spin->rect.width() / 3, Constants::spinIndicatorWidth);
                        QRect rect(spin->rect);
                        rect.setWidth(width);
                        rect.moveRight(spin->rect.right() - width);
                        return rect;
                    }
                    case SC_SpinBoxEditField: {
                        const int indicatorWidth = qMin(spin->rect.width() / 3, Constants::spinIndicatorWidth);
                        return spin->rect.adjusted(Constants::spinTextLeftPadding, 0, -(indicatorWidth * 2), 0);
                    }
                    default:
                        break;
                }
            }
            break;
        case CC_ComboBox:
            if (const auto* combo = qstyleoption_cast<const QStyleOptionComboBox*>(opt)) {
                switch (element) {
                    case SC_ComboBoxArrow: {
                        QRect rect(0, combo->rect.top(), Constants::comboArrowWidth, combo->rect.height());
                        rect.moveRight(combo->rect.right());
                        return rect;
                    }
                    case SC_ComboBoxEditField: {
                        return combo->rect.adjusted(Constants::comboTextLeftPadding, 0, -Constants::comboArrowWidth, 0);
                    }
                    case SC_ComboBoxFrame:
                        return combo->rect;

                    default:
                        break;
                }
            }
            break;
        case CC_ScrollBar:
            if (const QStyleOptionSlider* bar = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                switch (element) {
                    case SC_ScrollBarSubLine:
                    case SC_ScrollBarAddLine:
                    case SC_ScrollBarFirst:
                    case SC_ScrollBarLast:
                        return QRect();

                    case SC_ScrollBarGroove:
                        return bar->rect;

                    case SC_ScrollBarSlider: {
                        const int barLen = bar->orientation == Qt::Horizontal ? bar->rect.width() : bar->rect.height();
                        const int sliderLen = scrollbarGetSliderLength(bar);
                        int position = sliderPositionFromValue(bar->minimum,
                                                               bar->maximum,
                                                               bar->sliderPosition,
                                                               barLen - sliderLen,
                                                               bar->upsideDown);

                        if (bar->orientation == Qt::Horizontal) {
                            return QRect(bar->rect.left() + position, bar->rect.top(), sliderLen, bar->rect.height());
                        } else {
                            return QRect(bar->rect.left(), bar->rect.top() + position, bar->rect.width(), sliderLen);
                        }
                    }
                    case SC_ScrollBarSubPage: {
                        const int barLen = bar->orientation == Qt::Horizontal ? bar->rect.width() : bar->rect.height();
                        const int sliderLen = scrollbarGetSliderLength(bar);
                        int sliderPosition = sliderPositionFromValue(bar->minimum,
                                                                     bar->maximum,
                                                                     bar->sliderPosition,
                                                                     barLen - sliderLen,
                                                                     bar->upsideDown);

                        if (bar->orientation == Qt::Horizontal) {
                            return QRect(bar->rect.topLeft(), QPoint(bar->rect.left() + sliderPosition, bar->rect.bottom()));
                        } else {
                            return QRect(bar->rect.topLeft(), QPoint(bar->rect.right(), bar->rect.top() + sliderPosition));
                        }
                    }
                    case SC_ScrollBarAddPage: {
                        const int barLen = bar->orientation == Qt::Horizontal ? bar->rect.width() : bar->rect.height();
                        const int sliderLen = scrollbarGetSliderLength(bar);
                        int sliderPosition = sliderPositionFromValue(bar->minimum,
                                                                     bar->maximum,
                                                                     bar->sliderPosition,
                                                                     barLen - sliderLen,
                                                                     bar->upsideDown);
                        if (bar->orientation == Qt::Horizontal) {
                            return QRect(QPoint(bar->rect.left() + sliderPosition + sliderLen, bar->rect.top()), bar->rect.bottomRight());
                        } else {
                            return QRect(QPoint(bar->rect.left(), bar->rect.top() + sliderPosition + sliderLen), bar->rect.bottomRight());
                        }
                    }

                    default:
                        break;
                }
            }
        case CC_ToolButton:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
                switch (element) {
                    case SC_ToolButton:
                        if (btn->subControls & SC_ToolButtonMenu) {
                            return btn->rect.adjusted(0, 0, -Constants::toolbtnArrowSectionWidth, 0);
                        }
                        return btn->rect;
                    case SC_ToolButtonMenu:
                        if (btn->subControls & SC_ToolButtonMenu) {
                            QRect rect = btn->rect;
                            rect.setLeft(btn->rect.right() - Constants::toolbtnArrowSectionWidth);
                            return rect;
                        }
                        return btn->rect;
                    default:
                        break;
                }
            }
            break;
        case CC_GroupBox:
            if (const auto* box = qstyleoption_cast<const QStyleOptionGroupBox*>(opt)) {
                switch (element) {
                    case SC_GroupBoxCheckBox: {
                        if (!box->subControls & SC_GroupBoxCheckBox)
                            return QRect();

                        const QSize textSize = box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text);

                        QRect rect(0, box->rect.top(), Constants::groupBoxCheckSize, qMax(Constants::groupBoxCheckSize, textSize.height()));

                        if (box->textAlignment & Qt::AlignRight) {
                            if (textSize.width() > 0) {
                                rect.moveRight(box->rect.right() - Constants::groupBoxLabelOffset - textSize.width() - Constants::groupBoxTextCheckSpacing);
                            } else {
                                rect.moveRight(box->rect.right() - Constants::groupBoxLabelOffset);
                            }

                        } else if (box->textAlignment & Qt::AlignHCenter) {
                            if (textSize.width() > 0) {
                                rect.moveLeft(box->rect.toRectF().center().x() - ((rect.width() + Constants::groupBoxTextCheckSpacing + textSize.width()) / 2.0));
                            } else {
                                rect.moveLeft(box->rect.toRectF().center().x() - (rect.width() / 2.0));
                            }

                        } else {
                            rect.moveLeft(box->rect.left() + Constants::groupBoxLabelOffset);
                        }
                        return rect;
                    }
                    case SC_GroupBoxLabel: {
                        if (!box->subControls & SC_GroupBoxLabel)
                            return QRect();

                        const bool hasCheck = box->subControls & SC_GroupBoxCheckBox;
                        const QSize textSize = box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text);

                        QRect rect(0, box->rect.top(), textSize.width(), hasCheck ? qMax(Constants::groupBoxCheckSize, textSize.height()) : textSize.height());

                        if (box->textAlignment & Qt::AlignRight) {
                            rect.moveRight(box->rect.right() - Constants::groupBoxLabelOffset);

                        } else if (box->textAlignment & Qt::AlignHCenter) {
                            const int textWidth = box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text).width();
                            if (textWidth > 0) {
                                rect.moveRight(box->rect.toRectF().center().x() + ((rect.width() + Constants::groupBoxTextCheckSpacing + Constants::groupBoxCheckSize) / 2.0));
                            } else {
                                rect.moveRight(box->rect.toRectF().center().x() + (rect.width() / 2.0));
                            }

                        } else {
                            if (hasCheck) {
                                rect.moveLeft(box->rect.left() + Constants::groupBoxLabelOffset + Constants::groupBoxCheckSize + Constants::groupBoxTextCheckSpacing);
                            } else {
                                rect.moveLeft(box->rect.left() + Constants::groupBoxLabelOffset);
                            }
                        }
                        return rect;
                    }
                    case SC_GroupBoxFrame: {
                        const int labelHeight = (box->subControls & SC_GroupBoxLabel) ?
                                                    box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text).height() :
                                                    0;
                        const int checkHeight = (box->subControls & SC_GroupBoxCheckBox) ? Constants::groupBoxCheckSize : 0;
                        const int headerHeight = qMax(checkHeight, labelHeight); // height of the label area

                        return QRect(QPoint(box->rect.left(), box->rect.top() + headerHeight / 2), box->rect.bottomRight());
                    }
                    case SC_GroupBoxContents: {
                        const int labelHeight = (box->subControls & SC_GroupBoxLabel) ?
                                                    box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text).height() :
                                                    0;
                        const int checkHeight = (box->subControls & SC_GroupBoxCheckBox) ? Constants::groupBoxCheckSize : 0;
                        const int headerHeight = qMax(checkHeight, labelHeight); // height of the label area

                        return QRect(QPoint(box->rect.left() + box->lineWidth + 1, box->rect.top() + headerHeight),
                                     box->rect.bottomRight() - QPoint(box->lineWidth + 1, box->lineWidth + 1));
                    }
                    default:
                        break;
                }
            }
            break;
        case CC_Dial:
            if (const QStyleOptionSlider* dial = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                switch (element) {
                    case SC_DialGroove: {
                        const int size = qMin(dial->rect.width(), dial->rect.height());
                        QRect rect(0, 0, size, size);
                        rect.moveCenter(dial->rect.center());
                        return rect;
                    }
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

    return SuperStyle::subControlRect(cc, opt, element, widget);
}

QSize Style::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const {
    switch (ct) {
        case CT_PushButton: {
            const QSize original = SuperStyle::sizeFromContents(ct, opt, contentsSize, widget);
            const int heigth = qMax(opt->fontMetrics.height() + (2 * Constants::controlsTextHorizontalPadding),
                                    original.height());
            return QSize(original.width(), heigth);
        }

        case CT_LineEdit: {
            const int width = (opt->fontMetrics.averageCharWidth() * Constants::lineEditMinWidthChars) +
                              this->pixelMetric(PM_LineEditIconSize, opt, widget) +
                              (pixelMetric(PM_LineEditIconMargin, opt, widget) * 2);
            const int height = qMax(
                opt->fontMetrics.height() + (2 * Constants::controlsTextHorizontalPadding),
                this->pixelMetric(PM_LineEditIconSize, opt, widget) + (pixelMetric(PM_LineEditIconMargin, opt, widget) * 2));

            return QSize(width, height);
        }

        case CT_SpinBox:
            if (const auto* spin = qstyleoption_cast<const QStyleOptionSpinBox*>(opt)) {
                const int width = Constants::lineEditTextVerticalPadding +
                                  (spin->fontMetrics.averageCharWidth() * Constants::spinMinWidthChars) +
                                  (Constants::spinIndicatorWidth * 2);
                const int height = opt->fontMetrics.height() + (2 * Constants::controlsTextHorizontalPadding);
                return QSize(width, height);
            }
            break;

        case CT_ComboBox:
            if (const auto* combo = qstyleoption_cast<const QStyleOptionComboBox*>(opt)) {
                const int width = Constants::lineEditTextVerticalPadding +
                                  (combo->currentIcon.isNull() ? 0 : combo->iconSize.width() + Constants::lineEditTextVerticalPadding) +
                                  (combo->fontMetrics.averageCharWidth() * Constants::comboMinWidthChars) +
                                  Constants::comboArrowWidth;
                const int height = qMax(opt->fontMetrics.height() + (2 * Constants::controlsTextHorizontalPadding),
                                        combo->iconSize.height());
                return QSize(width, height);
            }
            break;

        case CT_RadioButton:
        case CT_CheckBox: {
            const QSize original = SuperStyle::sizeFromContents(ct, opt, contentsSize, widget);
            return QSize(original.width() + (Constants::checkBoxHoverCircleSize - Constants::checkBoxSize), qMax(Constants::checkBoxHoverCircleSize, original.height()));
        }
        case CT_MenuItem:
            if (const auto* menu = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
                switch (menu->menuItemType) {
                    case QStyleOptionMenuItem::Normal:
                    case QStyleOptionMenuItem::DefaultItem:
                    case QStyleOptionMenuItem::SubMenu: {

                        // textSize
                        QSize labelSize(0, 0);
                        QSize shortcutSize(0, 0);
                        const MenuItemText text = menuItemGetText(menu);
                        if (!text.label.isEmpty()) {
                            labelSize = menu->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), text.label);
                        }
                        if (!text.shortcut.isEmpty()) {
                            shortcutSize = menu->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), text.shortcut);
                        }

                        // width
                        int width = (Constants::menuItemVerticalExternalPadding * 2);
                        if (menu->menuHasCheckableItems) {
                            width += Constants::checkBoxHoverCircleSize;
                            width += Constants::menuHorizontalSpacing;
                        }
                        if (menu->maxIconWidth > 0) {
                            width += menu->maxIconWidth;
                            width += Constants::menuHorizontalSpacing;
                        }
                        if (labelSize.width() > 0) {
                            width += labelSize.width();
                            width += Constants::menuHorizontalSpacing;
                        }
                        if (menu->reservedShortcutWidth > 0) {
                            width += menu->reservedShortcutWidth;
                        }
                        if (menu->menuItemType == QStyleOptionMenuItem::SubMenu) {
                            width += Constants::smallArrowSize;
                        }

                        // height
                        const int heigth = qMax(labelSize.height(), shortcutSize.height()) + (Constants::menuItemHorizontalInternalPadding * 2);

                        return QSize(width, heigth);
                    }

                    case QStyleOptionMenuItem::Separator: {
                        const MenuItemText text = menuItemGetText(menu);
                        if (text.label.isEmpty()) {
                            // 1 is for the separator thickness
                            return QSize(Constants::menuSeparatorMinLen, 1 + (Constants::menuSeparatorHorizontalPadding * 2));
                        }
                        const QSize labelSize = menu->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), text.label);
                        const int width = (Constants::menuSeparatorHorizontalPadding * 2) +
                                          labelSize.width() +
                                          (Constants::menuSeparatorMinLen > 0 ? Constants::menuSeparatorMinLen + Constants::menuHorizontalSpacing : 0);

                        const int height = labelSize.height() + (Constants::menuItemHorizontalInternalPadding * 2);

                        return QSize(width, height % 2 == 1 ? height : height + 1); // height will always be odd
                    }

                    default:
                        break;
                }
            }
            break;
        case CT_MenuBarItem:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionMenuItem*>(opt)) {
                QSize textSize(0, 0);
                if (!bar->text.isEmpty()) {
                    textSize = bar->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), bar->text);
                }
                const int height = qMax(textSize.height(), Constants::menuBarItemMinHeight) + (Constants::menuBarItemExternalPadding * 2);

                int width = 0;
                width += textSize.width();
                if (!bar->icon.isNull()) {
                    const int iconWidth = qMin(this->pixelMetric(PM_SmallIconSize, bar, widget), height);
                    width += iconWidth;
                    if (!bar->text.isEmpty())
                        width += Constants::menuHorizontalSpacing;
                }
                if (width < 1) {
                    return QSize(0, 0);
                }
                width += (Constants::menuItemHorizontalInternalPadding * 2);
                width += (Constants::menuBarItemExternalPadding * 2);

                return QSize(width, height);
            }
            break;
        case CT_ScrollBar:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
                if (bar->orientation == Qt::Horizontal) {
                    return QSize(1, Constants::scrollBarThickness);
                }
                return QSize(Constants::scrollBarThickness, 1);
            }
            break;

        case CT_ToolButton:
            if (const auto* btn = qstyleoption_cast<const QStyleOptionToolButton*>(opt)) {
                QSize size(0, 0);
                switch (btn->toolButtonStyle) {
                    case Qt::ToolButtonIconOnly:
                        size = btn->iconSize;
                        break;
                    case Qt::ToolButtonTextOnly:
                        size = btn->fontMetrics.size(Qt::TextShowMnemonic, btn->text);
                        break;
                    case Qt::ToolButtonTextBesideIcon:
                    case Qt::ToolButtonFollowStyle: {
                        const QSize textSize = btn->fontMetrics.size(Qt::TextShowMnemonic, btn->text);
                        size.setWidth(btn->iconSize.width() + Constants::toolbtnLabelSpacing + textSize.width());
                        size.setHeight(qMax(btn->iconSize.height(), textSize.height()));
                    } break;
                    case Qt::ToolButtonTextUnderIcon: {
                        const QSize textSize = btn->fontMetrics.size(Qt::TextShowMnemonic, btn->text);
                        size.setWidth(qMax(btn->iconSize.width(), textSize.width()));
                        size.setHeight(btn->iconSize.height() + Constants::toolbtnLabelSpacing + textSize.height());
                    } break;
                }
                size += QSize(Constants::toolBtnLabelVerticalPadding * 2, Constants::toolBtnLabelHorizontalPadding * 2);

                if (btn->subControls & SC_ToolButtonMenu)
                    size += QSize(Constants::toolbtnArrowSectionWidth, 0);

                if (size.width() < size.height())
                    return QSize(size.height(), size.height());

                return size;
            }
            break;
        case CT_ProgressBar:
            if (const auto* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(opt)) {
                const bool horizontal = bar->state & State_Horizontal;

                if (!bar->textVisible) {
                    if (horizontal) {
                        return QSize(Constants::progressBarThickness * 4,
                                     Constants::progressBarThickness);
                    }
                    return QSize(Constants::progressBarThickness,
                                 Constants::progressBarThickness * 4);
                }

                const QSize textSizeDefault = bar->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), "100%");
                const QSize textSizeActual = bar->fontMetrics.size((Qt::TextSingleLine | Qt::TextShowMnemonic), bar->text);
                const QSize& textSize = textSizeDefault.width() > textSizeActual.width() ? textSizeDefault : textSizeActual;

                const QSize size(Constants::progressBarThickness * 4 + Constants::progressBarLabelVerticalPadding * 2 + textSize.width(),
                                 qMax(Constants::progressBarThickness, textSize.height()));
                if (horizontal) {
                    return size;
                }
                return size.transposed();
            }

        case CT_GroupBox:
            if (const auto* box = qstyleoption_cast<const QStyleOptionGroupBox*>(opt)) {
                const QSize labelSize = (box->subControls & SC_GroupBoxLabel) ?
                                            box->fontMetrics.size((Qt::TextHideMnemonic | Qt::TextSingleLine), box->text) :
                                            QSize(0, 0);
                const int checkSize = (box->subControls & SC_GroupBoxCheckBox) ? Constants::groupBoxCheckSize : 0;
                const int headerHeight = qMax(checkSize, labelSize.height()); // height of the label area

                const int minHeaderWidth = labelSize.width() + checkSize + ((labelSize.isValid() && checkSize > 0) ? Constants::groupBoxTextCheckSpacing : 0);

                const int lineThickness = (box->features & QStyleOptionFrame::Flat) ? 0 : box->lineWidth + 1;

                return QSize(qMax(contentsSize.width(), minHeaderWidth) + lineThickness * 2, contentsSize.height() + headerHeight + lineThickness);
            }

        case CT_SizeGrip:
            return QSize();

        default:
            break;
    }
    return SuperStyle::sizeFromContents(ct, opt, contentsSize, widget);
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

const Style::MenuItemText Style::menuItemGetText(const QStyleOptionMenuItem* menu) {
    const auto tabPosition = menu->text.lastIndexOf('\t');
    Style::MenuItemText text;
    if (tabPosition >= 0) {
        text.label = menu->text.first(tabPosition);
        if (menu->text.size() > tabPosition + 1)
            text.shortcut = menu->text.sliced(tabPosition + 1);
    } else {
        text.label = menu->text;
    }
    return text;
}

const int Style::scrollbarGetSliderLength(const QStyleOptionSlider* bar) const {
    const int barLen = bar->orientation == Qt::Horizontal ? bar->rect.width() : bar->rect.height();
    const int contentLen = bar->maximum - bar->minimum + bar->pageStep;
    const int minSliderLen = pixelMetric(PM_ScrollBarSliderMin, bar);
    if (contentLen <= 0) // to avoid division by 0
        return minSliderLen;
    const int sliderLen = qreal(bar->pageStep) / contentLen * barLen;

    return qMin(qMax(minSliderLen, sliderLen), barLen);
}

const int Style::getTextFlags(const QStyleOption* opt) const {
    int textFlags = Qt::TextShowMnemonic;
    if (!styleHint(SH_UnderlineShortcut, opt))
        textFlags |= Qt::TextHideMnemonic;
    return textFlags;
}

} // namespace Orchid
