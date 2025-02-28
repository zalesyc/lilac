#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "lilacsettingsapp.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QDialog dialog;
    dialog.setWindowTitle("Lilac style settings");

    QVBoxLayout mainLayout = QVBoxLayout(&dialog);

    Lilac::SettingsApp settingsApp = Lilac::SettingsApp(&dialog);
    mainLayout.addWidget(&settingsApp);

    QDialogButtonBox btnBox = QDialogButtonBox((QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults), &dialog);
    mainLayout.addWidget(&btnBox);

    QObject::connect(&btnBox, &QDialogButtonBox::accepted, &dialog, [&]() {
        settingsApp.save();
        dialog.close();
    });
    QObject::connect(&btnBox, &QDialogButtonBox::clicked, &dialog, [&](QAbstractButton* btn) {
        if (btnBox.buttonRole(btn) == QDialogButtonBox::ResetRole)
            settingsApp.defaults();
    });
    QObject::connect(&btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::close);
    QObject::connect(&dialog, &QDialog::finished, &app, &QApplication::quit);

    dialog.setLayout(&mainLayout);
    dialog.show();
    return app.exec();
}
