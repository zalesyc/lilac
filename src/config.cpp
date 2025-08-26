#if HAS_SETTINGS
#include "settings.h"
#endif

#include "config.h"

namespace Lilac {

Config::Config() {
#if HAS_SETTINGS
    initFromSettings();
#endif
}

Config& Config::get() {
    static Config instance;
    return instance;
}

#if HAS_SETTINGS
void Config::initFromSettings() {
    const auto settings = LilacSettings::self();
    cornerRadius = settings->cornerRadius();
    circleCheckBox = settings->circleCheckBox();
    menuBgOpacity = settings->menuOpacity();
    menuBlurBehind = settings->menuBlurBehind();
}
#endif

}  // namespace Lilac
