#if HAS_DBUS
#include <QDBusConnection>
#endif

#if HAS_SETTINGS
#include "settings.h"
#endif

#include "config.h"

namespace Lilac {

Config::Config() {
#if HAS_DBUS
    auto dbus = QDBusConnection::sessionBus();
    dbus.connect(
        "",
        "/LilacStyle",
        "com.github.zalesyc.lilac",
        "settingsChanged",
        this,
        SLOT(onSettingsChanged()));
    dbus.connect(
        "",
        "/KGlobalSettings",
        "org.kde.KGlobalSettings",
        "notifyChange",
        this,
        SLOT(onSettingsChanged()));
    dbus.connect(
        "",
        "/KWin",
        "org.kde.KWin",
        "reloadConfig",
        this,
        SLOT(onSettingsChanged()));
#endif
#if HAS_SETTINGS
    onSettingsChanged();
#endif
}

Config& Config::get() {
    return instance;
}

Config Config::instance;

#if HAS_SETTINGS
void Config::onSettingsChanged() {
    const auto settings = LilacSettings::self();
    settings->load();
    cornerRadius = settings->cornerRadius();
    circleCheckBox = settings->circleCheckBox();
    animationSpeed = settings->animationSpeed();
    menuBgOpacity = settings->menuOpacity();
    menuBlurBehind = settings->menuBlurBehind();
    emit configChanged();
}
#endif

}  // namespace Lilac
