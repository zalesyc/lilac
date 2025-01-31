// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#pragma once

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

#include "lilac.h"

namespace Lilac {

class Style : public SuperStyle {
    Q_OBJECT

   public:
    QString name() const { return QStringLiteral("Lilac"); }

    void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget = nullptr) const override;
    void drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget = nullptr) const override;
    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget = nullptr) const override;

    void polish(QWidget* widget) override;
    void unpolish(QWidget* widget) override;

    int pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(QStyle::StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
    QRect subElementRect(QStyle::SubElement element, const QStyleOption* option, const QWidget* widget = nullptr) const override;
    QRect subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl element, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget = nullptr) const override;

   private:
    struct MenuItemText {
        QString label = "";
        QString shortcut = "";
    };
    static const void sliderGetTickmarks(QList<QLine>* returnList, const QStyleOptionSlider* slider, const QRect& tickmarksRect, const int sliderLen, const int interval);
    static const MenuItemText menuItemGetText(const QStyleOptionMenuItem* menu);
    const int scrollbarGetSliderLength(const QStyleOptionSlider* bar) const;
    const int getTextFlags(const QStyleOption* opt) const;
    static QRect tabBarGetTabRect(const QStyleOptionTab* tab);
    static const bool tabIsHorizontal(const QTabBar::Shape& tabShape);
};
}  // namespace Lilac
