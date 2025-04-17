#include "lilacsettingsapp.h"
#include "ui_lilacsettingsapp.h"

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
    initFromSettings();
}

void SettingsApp::defaults() {
    settings->setDefaults();
    settings->save();
    initFromSettings();
}

void SettingsApp::save() {
    settings->setCornerRadius(ui->radiusSpin->value());
    settings->save();
#if HAS_DBUS
    auto msg = QDBusMessage::createSignal(
        "/LilacStyle",
        "com.github.zalesyc.lilacqt",
        "settingsChanged");
    QDBusConnection::sessionBus().send(msg);
#endif
}

void SettingsApp::initFromSettings() {
    settings->load();
    ui->radiusSpin->setValue(settings->cornerRadius());
    connect(ui->radiusSpin, &QSpinBox::valueChanged, this, [&](int newValue) { emit changed(newValue == settings->cornerRadius()); });
}

SettingsApp::~SettingsApp() {
    delete ui;
}

}  // namespace Lilac
