// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#include "lilacanimationmanager.h"
#include "lilac.h"

namespace Lilac {

AnimationManager::AnimationManager() {
    setGlobalAnimationSpeed(Config::defaultAnimationSpeed);
}

AnimationManager::~AnimationManager() {
}

void AnimationManager::remove(const QWidget* w) {
    if (!w || !animations.contains(w)) {
        return;
    }
    const QPointer<QVariantAnimation> ptr = animations.take(w);
    if (ptr) {
        delete ptr;
    }
}

void AnimationManager::setGlobalAnimationSpeed(const double speed) {
    if (speed <= 0) {
        durationMultiplier = 0;
        return;
    }

    durationMultiplier = 1.0 / speed;
}

QVariantAnimation* AnimationManager::getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, const int duration, const QVariantAnimation::Direction direction, bool infinite, bool independentOfAnimationSpeed) {
    QWidget* widget = const_cast<QWidget*>(w);
    if (!animations.contains(widget)) {
        animations.insert(widget, QPointer<QVariantAnimation>(new QVariantAnimation(this)));
    }
    QVariantAnimation* animation;
    animation = animations[widget].get();

    connect(widget, &QWidget::destroyed, this, [=]() { remove(widget); });
    connect(animation, SIGNAL(valueChanged(QVariant)), widget, SLOT(update()));

    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->setLoopCount(infinite ? -1 : 1);
    animation->setDuration(duration * (independentOfAnimationSpeed ? 1 : durationMultiplier));
    animation->setDirection(direction);
    if (animation->currentValue() != (direction == QVariantAnimation::Forward ? animation->endValue() : animation->startValue())) {
        animation->start();
    }

    return animation;
}

}  // namespace Lilac
