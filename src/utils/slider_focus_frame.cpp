// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#include <QEvent>
#include <QPointer>
#include <QTimer>

#include "slider_focus_frame.h"

namespace Lilac {

SliderFocusFrame::~SliderFocusFrame() {}

bool SliderFocusFrame::eventFilter(QObject* object, QEvent* event) {
    if (object == widget() && event->type() == QEvent::Paint) {
        // the QPointer is requred to chech that this is not deleted before the lambda
        QPointer<QFocusFrame> thisFrame = this;
        QTimer::singleShot(0, [thisFrame]() {if (thisFrame) thisFrame->update(); });
        return false;
    }
    return QFocusFrame::eventFilter(object, event);
}

}  // namespace Lilac
