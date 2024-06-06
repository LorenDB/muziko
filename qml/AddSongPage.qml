// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls.Material
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

        Label {
            text: qsTr("A song called \"%1\" already exists in your library").arg(name.text)
            color: Material.color(Material.Red)
            wrapMode: Label.WordWrap
            visible: Muziko.songs.containsSong(name.text)
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
        }

        RowLayout {
            Layout.columnSpan: 2
            spacing: 10

            Item { Layout.fillWidth: true }

            Button {
                enabled: name.text !== "" && !Muziko.songs.containsSong(name.text)
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
