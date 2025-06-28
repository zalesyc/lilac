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

    // w may not be nullptr
    QVariant getCurrentValue(const QWidget* w, const QVariant& start, const QVariant& end, bool infinite = false);

    void remove(const QWidget* w);

   private:
    QVariantAnimation* getOrCreateAnimation(const QWidget* w, const QVariant& start, const QVariant& end, bool infinite = false);

   private:
    QHash<const QWidget*, QPointer<QVariantAnimation>> animations;
};

}  // namespace Lilac
