// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#include "lilacanimationmanager.h"

namespace Lilac {

AnimationManager::AnimationManager() {
}
AnimationManager::~AnimationManager() {
    for (auto i = animations.begin(); i != animations.end(); i++) {
        if (i.value()) {
            delete i.value();
        }
    }
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

QVariantAnimation* AnimationManager::getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, const int duration, const QVariantAnimation::Direction direction, bool infinite) {
    QWidget* widget = const_cast<QWidget*>(w);

    if (!animations.contains(widget)) {
        animations.insert(widget, QPointer<QVariantAnimation>(new QVariantAnimation(widget)));
    }
    QVariantAnimation* animation;
    animation = animations[widget].get();

    connect(widget, &QVariantAnimation::destroyed, this, [=]() { remove(widget); });
    connect(animation, SIGNAL(valueChanged(QVariant)), widget, SLOT(update()));

    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->setLoopCount(infinite ? -1 : 1);
    animation->setDuration(duration);
    animation->setDirection(direction);
    if (animation->currentValue() != (direction == QVariantAnimation::Forward ? animation->endValue() : animation->startValue())) {
        animation->start();
    }

    return animation;
}

}  // namespace Lilac
