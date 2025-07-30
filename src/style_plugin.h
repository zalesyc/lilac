// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QStylePlugin>

namespace Lilac {
class StylePlugin : public QStylePlugin {
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "lilac.json")

   public:
    //* constructor
    explicit StylePlugin(QObject* parent = nullptr)
        : QStylePlugin(parent) {
    }

    //* returns list of valid keys
    QStringList keys() const;

    //* create style
    QStyle* create(const QString&) override;
};

}  // namespace Lilac
