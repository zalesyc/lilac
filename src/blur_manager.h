#pragma once

#if HAS_KWINDOWSYSTEM

#include <QEvent>
#include <QObject>
#include <QRegion>
#include <QWidget>

namespace Lilac {
class BlurManager : public QObject {
    Q_OBJECT
   public:
    explicit BlurManager(QObject* parent = nullptr);

    static bool shouldBlurBehindWidget(QWidget* Widget);

    void registerWidget(QWidget* widget);  // this does not check for shouldBlurBehindWidget
    void unregisterWidget(QWidget* widget);

   protected:
    static QRegion getBlurRegion(QWidget* widget);
    bool eventFilter(QObject* object, QEvent* event) override;

   public slots:
    void reapplyBlur();

   private:
    void enableBlur(QWidget* widget);

   private:
    QSet<QWidget*> regsteredWidgets;
};

}  // namespace Lilac

#endif
