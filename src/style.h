// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QCommonStyle>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

#if HAS_KSTYLE
#include <KStyle>
#endif

#include "animation_manager.h"
#include "config.h"

namespace Lilac {

#if HAS_KSTYLE
using SuperStyle = KStyle;
#else
using SuperStyle = QCommonStyle;
#endif

class Style : public SuperStyle {
    Q_OBJECT

#if HAS_KSTYLE
    Q_CLASSINFO("X-KDE-CustomElements", "true")
#endif

   public:
    QString name() const { return QStringLiteral("Lilac"); }
    Style();
    ~Style();
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

   protected:
    bool eventFilter(QObject* object, QEvent* event) override;

   protected:
    Lilac::Config& config;  // conveninece variable so Config::Get() does not have to be always called;
    mutable Lilac::AnimationManager animationMgr;

   private:
    struct MenuItemText {
        QString label = "";
        QString shortcut = "";
    };
    static void sliderGetTickmarks(QList<QLine>* returnList, const QStyleOptionSlider* slider, const QRect& tickmarksRect, const int sliderLen, const int interval);
    static MenuItemText menuItemGetText(const QStyleOptionMenuItem* menu);
    int scrollbarGetSliderLength(const QStyleOptionSlider* bar) const;
    int getTextFlags(const QStyleOption* opt) const;
    QRect tabBarGetTabRect(const QStyleOptionTab* tab) const;
    static bool tabIsHorizontal(const QTabBar::Shape& tabShape);
#if HAS_KWINDOWSYSTEM
    static inline bool shouldBlurBehindWidget(QWidget* Widget);
    QRegion getBlurRegion(QWidget* widget);
#endif
    static void drawDropShadow(QPainter* p, const QRectF& rect, const qreal cornerRadius, const qreal blurRadius, const QPointF offset, const QColor color);

   private:
#if HAS_KSTYLE
    ControlElement kstyle_CE_CapacityBar;
#endif
};
}  // namespace Lilac
