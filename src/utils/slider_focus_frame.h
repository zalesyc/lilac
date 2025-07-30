// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac contributors

#pragma once

#include <QFocusFrame>

namespace Lilac {

class SliderFocusFrame : public QFocusFrame {
    /* This class is used as a workaround for using FocusFrames with sliders (including dials).
     * By default QFocusFrame does not automatically update outside focused widget bounds during some operations,
     * which is a problem for sliders.
     */

    Q_OBJECT
   public:
    using QFocusFrame::QFocusFrame;
    ~SliderFocusFrame();

   protected:
    bool eventFilter(QObject* object, QEvent* event) override;
};

}  // namespace Lilac
