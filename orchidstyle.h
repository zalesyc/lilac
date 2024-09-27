#pragma once

// #include <QCommonStyle>
#include <QPainter>
#include <QPair>
#include <QProxyStyle>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleOption>
#include <QWidget>

#include "orchid.h"

namespace Orchid {

class Style : public SuperStyle {
    Q_OBJECT

  public:
    // Style() : QProxyStyle(getStyle()) { // uncommnent this when using qproxystyle
    //     qDebug() << QStyleFactory::keys();
    // }
    QString name() const {
        return QLatin1StringView("Orchid");
    }
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

    // int combinedLayoutSpacing(QSizePolicy::ControlTypes controls1, QSizePolicy::ControlTypes controls2, Qt::Orientation orientation, QStyleOption* opt = nullptr, QWidget* widget = nullptr) const;
    // int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;

  protected:
    static const QString getStyle();

  private:
    struct MenuItemText {
        QString label = "";
        QString shortcut = "";
    };
    static const void sliderGetTickmarks(QList<QLine>* returnList, const QStyleOptionSlider* slider, const QRect& tickmarksRect, const int sliderLen, const int interval);
    static const MenuItemText menuItemGetText(const QStyleOptionMenuItem* menu);
    const int scrollbarGetSliderLength(const QStyleOptionSlider* bar) const;
};
} // namespace Orchid
