// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 zalesyc and the lilac-qt contributors

#pragma once

#include <QStyle>

namespace Lilac {

struct State {
    State() {}
    State(const QStyle::State& state) {
        enabled = state & QStyle::State_Enabled;
        hovered = state & QStyle::State_MouseOver;
        pressed = state & QStyle::State_Sunken;
        hasFocus = state & QStyle::State_HasFocus;
    }

    bool enabled = true;    // QStyle::State_Enabled
    bool hovered = false;   // QStyle::State_MouseOver
    bool pressed = false;   // QStyle::State_Sunken
    bool hasFocus = false;  // QStyle::State_HasFocus
};

}  // namespace Lilac
