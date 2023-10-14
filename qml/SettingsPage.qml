// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import dev.lorendb.muziko

Page {
    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2
        rowSpacing: 10
        columnSpacing: 10

        Label {
            text: qsTr("Daily practice set size")
        }

        SpinBox {
            from: 1
            to: 100
            value: Settings.dailySetSize
            onValueChanged: Settings.dailySetSize = value
            Layout.alignment: Qt.AlignRight
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
        }
    }
}
