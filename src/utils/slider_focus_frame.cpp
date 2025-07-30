// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#include <QEvent>
#include <QTimer>

#include "slider_focus_frame.h"

namespace Lilac {

SliderFocusFrame::~SliderFocusFrame() {}

bool SliderFocusFrame::eventFilter(QObject* object, QEvent* event) {
    if (object == widget() && event->type() == QEvent::Paint) {
        QTimer::singleShot(0, [this]() { this->update(); });
        return false;
    }
    return QFocusFrame::eventFilter(object, event);
}

}  // namespace Lilac
