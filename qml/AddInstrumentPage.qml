// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2
        rowSpacing: 10
        columnSpacing: 10

        Label {
            text: qsTr("Instrument name")
        }

        TextField {
            id: name

            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: parent.width * 0.6
            placeholderText: qsTr("Instrument name")
        }

        Item {
            Layout.fillHeight: true
            Layout.columnSpan: 2
        }

        RowLayout {
            Layout.columnSpan: 2
            spacing: 10

            Item { Layout.fillWidth: true }

            Button {
                enabled: name.text !== "" && !Muziko.alreadyHasInstrument(name.text)
                text: qsTr("OK")
                onClicked: {
                    Muziko.addInstrument(name.text);
                    songsStack.pop();
                }
            }

            Button {
                text: qsTr("Cancel")
                visible: Muziko.songs != null
                onClicked: songsStack.pop()
            }
        }
    }
}
