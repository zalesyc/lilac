// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

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
    T getCurrentValue(const QWidget* w, const T& start, const T& end, const int duration, const QVariantAnimation::Direction direction = QVariantAnimation::Forward, bool infinite = false) {
        return getOrCreateAnimation(w, start, end, duration, direction, infinite)->currentValue().template value<T>();
    }

    // this returns only the value of the animation, if the animation doesnt exist it returns defaultValue
    // T must be the same as where the animation was defined, othervise defaultValue is returned
    template <typename T>
    T getOnlyValue(const QWidget* w, const T defaultValue = T()) {
        if (!w || !animations.contains(w)) {
            return defaultValue;
        }
        const QVariant variant = animations[w]->currentValue();
        if (!variant.template canConvert<T>()) {
            return defaultValue;
        }
        return variant.template value<T>();
    }

    void remove(const QWidget* w);

   private:
    QVariantAnimation* getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, const int duration, const QVariantAnimation::Direction direction, bool infinite);

   private:
    QHash<const QWidget*, QPointer<QVariantAnimation>> animations;
};

}  // namespace Lilac
