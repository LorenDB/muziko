// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import dev.lorendb.muziko

Page {
    id: editSongRoot

    required property Song song

    MessageDialog {
        id: deleteDialog

        buttons: MessageDialog.Ok | MessageDialog.Cancel
        text: qsTr("Are you sure you want to delete %1?").arg(editSongRoot.song.name)
        onAccepted: {
            Muziko.songs.removeSong(editSongRoot.song.name);
            stack.pop();
        }
    }

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
            // TODO: handle "Invalid Date"
            text: editSongRoot.song.lastPracticed
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
                    stack.pop();
                }
            }

            Button {
                text: qsTr("Cancel")
                onClicked: stack.pop()
            }
        }
    }
}
