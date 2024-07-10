#include "orchidstyleplugin.h"
#include "orchidstyle.h"
#include <QApplication>
#include <QObject>
#include <QStringList>
#include <QStringLiteral>

namespace Orchid {
QStyle* StylePlugin::create(const QString& key) {
    if (key.compare(QLatin1String("orchid"), Qt::CaseInsensitive) == 0) {
        return new Style;
    }
    return nullptr;
}

//_________________________________________________
QStringList StylePlugin::keys() const {
    return QStringList(QStringLiteral("Orchid"));
}
} // namespace Orchid
