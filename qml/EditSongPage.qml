// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    id: editSongRoot

    required property Song song

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
            text: editSongRoot.song.name
        }

        Label {
            text: qsTr("Last practiced on")
        }

        Label {
            Layout.alignment: Qt.AlignRight
            text: editSongRoot.song.lastPracticedString
        }

        Label {
            text: qsTr("Your current skill level")
        }

        Slider {
            id: proficiency

            from: 0
            to: 2
            value: editSongRoot.song.proficiency
            stepSize: 1
            snapMode: Slider.SnapAlways
            Layout.alignment: Qt.AlignRight
        }

        Item {}

        Button {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Delete")
            icon.source: Qt.resolvedUrl("icons/delete.svg")
            icon.color: Material.color(Material.Red)

            onClicked: deleteDialog.open()
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
                    editSongRoot.song.name = name.text;
                    editSongRoot.song.proficiency = proficiency.value;
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
