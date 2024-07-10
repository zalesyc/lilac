#pragma once

#include <QStylePlugin>

namespace Orchid {
class StylePlugin : public QStylePlugin {
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "orchid.json")

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

} // namespace Orchid
