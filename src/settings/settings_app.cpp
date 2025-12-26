// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#include "settings_app.h"
#include "ui_settings_app.h"

#if HAS_DBUS
#include <QDBusConnection>
#include <QDBusMessage>
#endif

extern "C" {
Q_DECL_EXPORT QWidget* allocate_kstyle_config(QWidget* parent) {
    return new Lilac::SettingsApp(parent);
}
}

namespace Lilac {
SettingsApp::SettingsApp(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::lilacsettingsapp),
      settings(LilacSettings::self()) {
    ui->setupUi(this);

    connect(ui->radiusSpin, &QSpinBox::valueChanged, this, &SettingsApp::widgetChanged);
    connect(ui->circleCheckCheckBox, &QCheckBox::clicked, this, &SettingsApp::widgetChanged);
    connect(ui->spinVerticalControlsCheck, &QCheckBox::clicked, this, &SettingsApp::widgetChanged);
    connect(ui->animationSpeedSlider, &QSlider::valueChanged, this, &SettingsApp::widgetChanged);
    connect(ui->menuOpacitySlider, &QSlider::valueChanged, this, &SettingsApp::widgetChanged);
    connect(ui->blurBehindMenusCheck, &QCheckBox::clicked, this, &SettingsApp::widgetChanged);
    connect(ui->menuOutlineCheck, &QCheckBox::clicked, this, &SettingsApp::widgetChanged);
    connect(ui->tabAlignmentComboBox, &QComboBox::currentIndexChanged, this, &SettingsApp::widgetChanged);
    connect(ui->groupBoxAltStyleCheck, &QCheckBox::clicked, this, &SettingsApp::widgetChanged);
    connect(ui->menuOpacitySlider, &QSlider::valueChanged, this, [this](int value) { ui->blurBehindMenusCheck->setEnabled(HAS_KWINDOWSYSTEM && value < 255); });

    loadFromSettings();
}

void SettingsApp::defaults() {
    settings->setDefaults();
    settings->save();
    loadFromSettings();
}

void SettingsApp::save() {
    settings->setCornerRadius(ui->radiusSpin->value());
    settings->setCircleCheckBox(ui->circleCheckCheckBox->isChecked());
    const int animationSpeed = ui->animationSpeedSlider->value();
    settings->setAnimationSpeed(animationSpeed > 99 ? 0 : qMax(animationSpeed, 1) / 10.0);
    settings->setMenuOpacity(ui->menuOpacitySlider->value());
    settings->setMenuBlurBehind(ui->blurBehindMenusCheck->isChecked());
    settings->setSpinBoxVerticalControls(ui->spinVerticalControlsCheck->isChecked());
    settings->setTabBarTabContentAlignment(ui->tabAlignmentComboBox->currentIndex());
    settings->setMenuDrawOutline(ui->menuOutlineCheck->isChecked());
    settings->setGroupBoxAltStyle(ui->groupBoxAltStyleCheck->isChecked());
    settings->save();
#if HAS_DBUS
    auto msg = QDBusMessage::createSignal(
        "/LilacStyle",
        "com.github.zalesyc.lilac",
        "settingsChanged");
    QDBusConnection::sessionBus().send(msg);
#endif
}

void SettingsApp::loadFromSettings() {
    settings->load();
    ui->radiusSpin->setValue(settings->cornerRadius());
    ui->circleCheckCheckBox->setChecked(settings->circleCheckBox());
    ui->spinVerticalControlsCheck->setChecked(settings->spinBoxVerticalControls());
    const qreal animationSpeed = settings->animationSpeed();
    ui->animationSpeedSlider->setValue(animationSpeed <= 0 ? 100 : (animationSpeed * 10.0));
    ui->menuOpacitySlider->setValue(settings->menuOpacity());
    ui->blurBehindMenusCheck->setChecked(settings->menuBlurBehind());
    ui->tabAlignmentComboBox->setCurrentIndex(settings->tabBarTabContentAlignment());
    ui->menuOutlineCheck->setChecked(settings->menuDrawOutline());
    ui->groupBoxAltStyleCheck->setChecked(settings->groupBoxAltStyle());
}

SettingsApp::~SettingsApp() {
    delete ui;
}

void SettingsApp::widgetChanged() {
    if (alreadyChanged) {
        return;
    }
    emit changed(true);
}

}  // namespace Lilac
