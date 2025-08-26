// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include "settings.h"

#include <QWidget>

namespace Ui {
class lilacsettingsapp;
}
namespace Lilac {
class SettingsApp : public QWidget {
    Q_OBJECT

   public:
    explicit SettingsApp(QWidget* parent = nullptr);
    ~SettingsApp();

   public slots:
    void defaults();
    void save();

   signals:
    void changed(bool changed);

   private:
    void setFromSettings();

   private slots:
    void widgetChanged();

   private:
    Ui::lilacsettingsapp* ui;
    LilacSettings* settings;
    bool alreadyChanged = false;
};
}  // namespace Lilac
