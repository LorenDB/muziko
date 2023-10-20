// SPDX-FileCopyrightText: Loren Burkholder
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls

ToolButton {
    id: checkboxRoot

    checkable: true
    background: Rectangle {
        radius: width / 2
        border.color: "#ffffff"
        border.width: checkboxRoot.checked ? 0 : 1
        color: checkboxRoot.checked ? null : "transparent"
        gradient: checkboxRoot.checked ? muzikoGradient : null
        rotation: 45
    }
    // the null SVG is necessary because the ToolButton will render at a different size with no icon set
    icon.source: Qt.resolvedUrl(checked ? "icons/done.svg" : "icons/null.svg")
}
