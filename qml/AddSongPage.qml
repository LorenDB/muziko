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
            text: qsTr("Song name")
        }

        TextField {
            id: name

            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: parent.width * 0.6
            placeholderText: qsTr("Song name")
        }

        Label {
            text: qsTr("Your current skill level")
        }

        Slider {
            id: proficiency

            from: 0
            to: 2
            stepSize: 1
            snapMode: Slider.SnapAlways
            Layout.alignment: Qt.AlignRight
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
                enabled: name.text !== ""
                text: qsTr("OK")
                onClicked: {
                    Muziko.songs.addSong(name.text, proficiency.value)
                    songsStack.pop();
                }
            }

            Button {
                text: qsTr("Cancel")
                onClicked: songsStack.pop()
            }
        }
    }
}
