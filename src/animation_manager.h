// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QHash>
#include <QPointer>
#include <QVariantAnimation>
#include <QWidget>

namespace Lilac {

class AnimationManager : QObject {
    Q_OBJECT

   public:
    AnimationManager();
    ~AnimationManager();

    // w may not be nullptr
    template <typename T>
    T getCurrentValue(const QWidget* w, const T& start, const T& end, const int duration, const QVariantAnimation::Direction direction = QVariantAnimation::Forward, bool infinite = false, bool independentOfAnimationSpeed = false) {
        if (!independentOfAnimationSpeed && durationMultiplier == 0) {
            return end;
        }
        return getOrCreateAnimation(w, start, end, duration, direction, infinite, independentOfAnimationSpeed)->currentValue().template value<T>();
    }

    // this returns only the value of the animation, if the animation doesnt exist it returns defaultValue
    // T must be the same as where the animation was defined, othervise defaultValue is returned
    template <typename T>
    T getOnlyValue(const QWidget* w, const T defaultValue = T()) {
        if (!w || !animations.contains(w) || durationMultiplier == 0) {
            return defaultValue;
        }
        const QVariant variant = animations[w]->currentValue();
        if (!variant.template canConvert<T>()) {
            return defaultValue;
        }
        return variant.template value<T>();
    }

    void remove(const QWidget* w);

    void setGlobalAnimationSpeed(const double speed);

   private:
    QVariantAnimation* getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, const int duration, const QVariantAnimation::Direction direction, bool infinite, bool independentOfAnimationSpeed);

   private:
    QHash<const QWidget*, QPointer<QVariantAnimation>> animations;
    double durationMultiplier = 1;  // global amimation speed, default value set in constructor
};

}  // namespace Lilac
