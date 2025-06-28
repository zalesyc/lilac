// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#include "lilacanimationmanager.h"
// #define T int
namespace Lilac {

AnimationManager::AnimationManager() {
}

QVariant AnimationManager::getCurrentValue(const QWidget* w, const QVariant& start, const QVariant& end, bool infinite) {
    return getOrCreateAnimation(w, start, end, infinite)->currentValue();
}

void AnimationManager::remove(const QWidget* w) {
    qDebug() << "destroying" << w << animations.contains(w);
    if (!w || !animations.contains(w)) {
        return;
    }
    const QPointer<QVariantAnimation> ptr = animations.take(w);
    if (ptr) {
        delete ptr;
    }
}

QVariantAnimation* AnimationManager::getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, bool infinite) {
    QWidget* widget = const_cast<QWidget*>(w);

    if (animations.contains(widget)) {
        return animations[widget].get();
    }

    QVariantAnimation* animation;
    animations.insert(widget, QPointer<QVariantAnimation>(new QVariantAnimation(widget)));
    animation = animations[widget].get();

    connect(widget, &QVariantAnimation::destroyed, this, [=]() { remove(widget); });
    connect(animation, SIGNAL(valueChanged(QVariant)), widget, SLOT(update()));

    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->setLoopCount(infinite ? -1 : 0);
    animation->setDuration(2000);
    animation->start();

    return animation;
}

}  // namespace Lilac
