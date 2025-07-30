// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#include <QStringList>
#include <QStringLiteral>

#include "style.h"
#include "style_plugin.h"

namespace Lilac {
QStyle* StylePlugin::create(const QString& key) {
    if (key.compare(QLatin1String("lilac"), Qt::CaseInsensitive) == 0) {
        return new Style;
    }
    return nullptr;
}

QStringList StylePlugin::keys() const {
    return QStringList(QStringLiteral("Lilac"));
}
}  // namespace Lilac
