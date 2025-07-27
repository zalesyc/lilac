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
    connect(ui->animationSpeedSlider, &QSlider::valueChanged, this, [this](int value) { ui->animationValue->setText(QString::number(value / 10.0)); });

    setFromSettings();
}

void SettingsApp::defaults() {
    settings->setDefaults();
    settings->save();
    setFromSettings();
}

void SettingsApp::save() {
    settings->setCornerRadius(ui->radiusSpin->value());
    settings->setCircleCheckBox(ui->circleCheckCheckBox->isChecked());
    settings->setAnimationSpeed(ui->animationSpeedSlider->value() / 10.0);
    settings->save();
#if HAS_DBUS
    auto msg = QDBusMessage::createSignal(
        "/LilacStyle",
        "com.github.zalesyc.lilacqt",
        "settingsChanged");
    QDBusConnection::sessionBus().send(msg);
#endif
}

void SettingsApp::setFromSettings() {
    settings->load();
    ui->radiusSpin->setValue(settings->cornerRadius());
    ui->circleCheckCheckBox->setChecked(settings->circleCheckBox());
    ui->animationSpeedSlider->setValue(settings->animationSpeed() * 10.0);
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
