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
                if (state.hovered) {
                    p->save();
                    p->setPen(getPen(opt->palette, Color::scrollBarHoverOutline, 1));
                    p->setBrush(Qt::NoBrush);
                    p->fillRect(bar->rect, getBrush(opt->palette, Color::scrollBarHoverBackground));
                    if (opt->state & QStyle::State_Horizontal) {
                        p->drawLine(bar->rect.topLeft(), bar->rect.topRight());
                        p->drawLine(bar->rect.bottomLeft(), bar->rect.bottomRight());
                    } else {
                        p->drawLine(bar->rect.topLeft(), bar->rect.bottomLeft());
                        p->drawLine(bar->rect.topRight(), bar->rect.bottomRight());
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

        case CC_ComboBox:
            if (const auto combo = qstyleoption_cast<const QStyleOptionComboBox*>(opt)) {
                state.pressed = (combo->state & State_On);
                p->save();
                p->setRenderHint(QPainter::Antialiasing);
                p->setPen(getPen(combo->palette, Color::comboBoxOutline, state, 2));
                p->setBrush(getBrush(combo->palette, Color::comboBoxBackground, state));
                p->drawRoundedRect(combo->rect.adjusted(1, 1, -1, -1), Constants::btnRadius, Constants::btnRadius);
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
                        cornerRectSize = qMin(Constants::btnRadius * 2, // the corner radius is still btnRadius,
                                              qMin(rect.width(),        // it has to be *2 due to implementation
                                                   rect.height() * 2));

                        break;

                    case QTabBar::RoundedEast:
                    case QTabBar::TriangularEast:
                    case QTabBar::RoundedWest:
                    case QTabBar::TriangularWest:
                        cornerRectSize = qMin(Constants::btnRadius * 2,
                                              qMin(rect.height(),
                                                   rect.width() * 2));
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
            if (state.hovered) {
                drawControl(QStyle::CE_ScrollBarAddPage, opt, p, widget);
            }
            QRect rect;
            const int gapSize = state.hovered ? Constants::scrollBarSliderPaddingHover : Constants::scrollBarSliderPadding;
            if (opt->state & QStyle::State_Horizontal) {
                rect = opt->rect.adjusted(0, gapSize, 0, -gapSize);
            } else {
                rect = opt->rect.adjusted(gapSize, 0, -gapSize, 0);
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
                            p->setBrush(getBrush(menu->palette, Color::menuItemHoverBackground, state));
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
                            int textFlags = Qt::TextShowMnemonic;
                            if (!styleHint(SH_UnderlineShortcut, menu, widget))
                                textFlags |= Qt::TextHideMnemonic;

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
                            int textFlags = Qt::TextShowMnemonic;
                            if (!styleHint(SH_UnderlineShortcut, menu, widget))
                                textFlags |= Qt::TextHideMnemonic;

                            const QSize labelSize = menu->fontMetrics.size((Qt::TextShowMnemonic | Qt::AlignLeft | Qt::AlignVCenter), text.label);
                            const QRect labelRect(menu->rect.left() + Constants::menuItemVerticalExternalPadding, menu->rect.top(), labelSize.width(), menu->rect.height());
                            p->setPen(getPen(menu->palette, Color::menuText));
                            p->setFont(menu->font);
                            p->drawText(labelRect, (textFlags | Qt::AlignLeft | Qt::AlignVCenter), menu->text);

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
                    int textFlags = Qt::TextShowMnemonic;
                    if (!styleHint(SH_UnderlineShortcut, bar, widget))
                        textFlags |= Qt::TextHideMnemonic;

                    QRect labelRect(0, contentsRect.top(), 0, contentsRect.height());
                    labelRect.setLeft(
                        contentsRect.left() + Constants::menuItemVerticalInternalPadding + (iconWidth > 0 ? iconWidth + Constants::menuHorizontalSpacing : 0));
                    labelRect.setRight(
                        contentsRect.right() - Constants::menuItemVerticalInternalPadding);
                    p->drawText(labelRect, (Qt::AlignLeft | Qt::AlignVCenter | textFlags | Qt::TextSingleLine), bar->text);
                }
                p->restore();
                return;
            }
            break;

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
                    int textFlags = Qt::TextShowMnemonic;
                    if (!styleHint(SH_UnderlineShortcut, combo, widget))
                        textFlags |= Qt::TextHideMnemonic;

                    QRect textRect(labelRect);
                    textRect.setLeft(labelRect.left() + (hasIcon ? combo->iconSize.width() + Constants::lineEditTextVerticalPadding : 0));

                    p->save();
                    p->setPen(getPen(combo->palette, Orchid::comboBoxUneditableText));
                    p->setBrush(Qt::NoBrush);
                    p->drawText(textRect, (textFlags | Qt::AlignLeft | Qt::AlignVCenter), combo->currentText);
                    p->restore();
                }
                return;
            }
            break;

        case CE_MenuBarEmptyArea:
            return;

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
            p->setPen(getPen(opt->palette, Color::focusColor, 2));
            p->drawRoundedRect(opt->rect.adjusted(1, 1, -1, -1), Constants::btnRadius, Constants::btnRadius);
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
                p->setPen(getPen(opt->palette, Color::checkBoxHoverCircle, state, indicatorRect.left() - hoverRect.left()));
                p->setBrush(Qt::NoBrush);
                const int adjustment = ((indicatorRect.left() - hoverRect.left()) / 2) + 1;
                p->drawEllipse(hoverRect.adjusted(adjustment, adjustment, -adjustment, -adjustment));
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
                    const QPointF poinsts[3] = {
                        QPointF(center.x() - indicatorSize / 4.0f, center.y()),
                        QPointF(center.x() - indicatorSize / 10.0f, center.y() + indicatorSize / 5.0f),
                        QPointF(center.x() + indicatorSize / 4.0f, center.y() - indicatorSize / 7.5f),
                    };

                    p->drawPolyline(poinsts, 3);
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
            p->setPen(getPen(opt->palette, Color::lineEditOutline, state, 2));
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
            p->setBrush(getBrush(opt->palette, Color::menuBackground, state));
            p->drawRoundedRect(opt->rect.adjusted(Constants::menuTransparentPadding,
                                                  Constants::menuTransparentPadding,
                                                  -Constants::menuTransparentPadding,
                                                  -Constants::menuTransparentPadding),
                               Constants::menuBorderRadius,
                               Constants::menuBorderRadius);
            p->restore();
            return;
        }

        case PE_FrameMenu:
            return;

            // case PE_IndicatorArrowUp:    // ---> this works ok but there are some issues,
            // case PE_IndicatorArrowDown: // QCommonstyle implementation is good except for disabled colors
            // case PE_IndicatorArrowLeft:
            // case PE_IndicatorArrowRight: {
            //     const int size = qMin(opt->rect.width(), opt->rect.height());
            //     QRect rect;
            //     if (element == PE_IndicatorArrowUp || element == PE_IndicatorArrowDown) {
            //         rect.setWidth(size);
            //         rect.setHeight(size / 2);
            //     } else {
            //         rect.setHeight(size);
            //         rect.setWidth(size / 2);
            //     }
            //     rect.moveCenter(opt->rect.center());
            //     QPoint points[3];
            //     switch (element) {
            //         case PE_IndicatorArrowUp:
            //             points[0] = rect.bottomLeft();
            //             points[1] = QPoint(rect.center().x(), rect.top());
            //             points[2] = rect.bottomRight();
            //             break;
            //         case PE_IndicatorArrowDown:
            //             points[0] = rect.topLeft();
            //             points[1] = QPoint(rect.center().x(), rect.bottom());
            //             points[2] = rect.topRight();
            //             break;
            //         case PE_IndicatorArrowRight:
            //             points[0] = rect.topLeft();
            //             points[1] = QPoint(rect.right(), rect.center().y() + 1);
            //             points[2] = rect.bottomLeft();
            //             break;
            //         case PE_IndicatorArrowLeft:
            //             points[0] = rect.topRight();
            //             points[1] = QPoint(rect.left(), rect.center().y());
            //             points[2] = rect.bottomRight();
            //             break;
            //         default:
            //             break;
            //     }
            //     p->save();
            //     // p->setRenderHints(QPainter::Antialiasing);
            //     p->setPen(Qt::NoPen);
            //     p->setBrush(getBrush(opt->palette, Color::indicatorArrow, state));
            //     p->drawPolygon(points, 3);
            //     p->restore();
            //     return;
            // }

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
            return 0;
        case PM_MenuBarPanelWidth:
            return 0;
        case PM_ScrollView_ScrollBarOverlap:
            return Constants::scrollBarThickness;
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

        case CT_SizeGrip:
            return QSize();

        default:
            break;
    }
    return SuperStyle::sizeFromContents(ct, opt, contentsSize, widget);
}

const QString Style::getStyle() { // this is not ideal, but shold work - todo: make this configurable in settings
    const QStringList availibleStyles = QStyleFactory::keys();
    if (availibleStyles.contains("", Qt::CaseInsensitive)) {
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

} // namespace Orchid
