#if HAS_KWINDOWSYSTEM

#include <KWindowEffects>

#include "blur_manager.h"
#include "config.h"

namespace Lilac {

BlurManager::BlurManager(QObject* parent)
    : QObject(parent) {
    connect(&Config::get(), &Config::configChanged, this, &BlurManager::reapplyBlur);
}

bool BlurManager::shouldBlurBehindWidget(QWidget* widget) {
    return widget->inherits("QMenu");
}
void BlurManager::registerWidget(QWidget* widget) {
    widget->installEventFilter(this);
    regsteredWidgets.insert(widget);
    connect(widget, &QWidget::destroyed, this, [widget, this]() { this->regsteredWidgets.remove(widget); });
}

void BlurManager::unregisterWidget(QWidget* widget) {
    widget->removeEventFilter(this);
    regsteredWidgets.remove(widget);
}

QRegion BlurManager::getBlurRegion(QWidget* widget) {
    if (widget->inherits("QMenu")) {
        const Config& config = Config::get();
        QRegion region;
        const QRect innerRect = widget->rect().adjusted(Config::menuMargin, Config::menuMargin, -Config::menuMargin, -Config::menuMargin);
        region += innerRect.adjusted(config.menuBorderRadius, 0, -config.menuBorderRadius, 0);
        region += innerRect.adjusted(0, config.menuBorderRadius, 0, -config.menuBorderRadius);
        region += QRegion(QRect(innerRect.topLeft(), QSize(config.menuBorderRadius, config.menuBorderRadius) * 2).normalized(), QRegion::Ellipse);
        region += QRegion(QRect(innerRect.bottomLeft(), QSize(config.menuBorderRadius, -config.menuBorderRadius) * 2).normalized(), QRegion::Ellipse);
        region += QRegion(QRect(innerRect.topRight(), QSize(-config.menuBorderRadius, config.menuBorderRadius) * 2).normalized(), QRegion::Ellipse);
        region += QRegion(QRect(innerRect.bottomRight(), QSize(-config.menuBorderRadius, -config.menuBorderRadius) * 2).normalized(), QRegion::Ellipse);
        return region;
    }
    return widget->rect();
}

bool BlurManager::eventFilter(QObject* object, QEvent* event) {
    QWidget* widget = qobject_cast<QWidget*>(object);
    if (!widget) {
        return QObject::eventFilter(object, event);
    }
    const QEvent::Type eventType = event->type();
    if (eventType != QEvent::Hide && eventType != QEvent::Show && eventType != QEvent::Resize) {
        return QObject::eventFilter(object, event);
    }
    enableBlur(widget);
    return false;
}

void BlurManager::reapplyBlur() {
    for (const auto& widget : regsteredWidgets) {
        enableBlur(widget);
    }
}

void BlurManager::enableBlur(QWidget* widget) {
    const Config& config = Config::get();

    if (config.menuBlurBehind &&
        config.menuBgOpacity != 255 &&
        (widget->testAttribute(Qt::WA_WState_Created) || widget->internalWinId())) {
        widget->winId();
        KWindowEffects::enableBlurBehind(widget->windowHandle(), true, getBlurRegion(widget));
        if (widget->isVisible()) {
            widget->update();
        }
    }
}
}  // namespace Lilac
#endif
