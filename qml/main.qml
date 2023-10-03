// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: "Muziko"

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 10

            Item { Layout.fillWidth: true }

            Label { text: Muziko.model.instrument }

            Item { Layout.fillWidth: true }
        }
    }
}
