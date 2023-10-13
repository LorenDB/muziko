// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

ListView {
    id: songList

    property bool showAll: false

    model: showAll ? Muziko.songs : Muziko.songsFilter
    boundsBehavior: Flickable.OvershootBounds
    remove: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 75
        }
    }
    displaced: Transition {
        NumberAnimation {
            properties: "x,y"
            duration: 150
        }
    }

    delegate: ItemDelegate {
        id: del

        required property Song song

        width: songList.width
        height: editBtn.implicitHeight + 20

        RowLayout {
            id: row

            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Label {
                text: del.song.name
                elide: Text.ElideRight
            }

            Item { Layout.fillWidth: true }

            Label {
                text: del.song.proficiencyString
                color: {
                    if (del.song.proficiency == Song.LowProficiency)
                        return Material.color(Material.Red);
                    else if (del.song.proficiency == Song.MediumProficiency)
                        return Material.color(Material.Orange);
                    else if (del.song.proficiency == Song.HighProficiency)
                        return Material.color(Material.Green);
                    else
                        return palette.text;
                }
            }

            ToolButton {
                icon.source: Qt.resolvedUrl("icons/done.svg")
                onClicked: del.song.markAsPracticedToday()
            }

            ToolButton {
                id: editBtn

                icon.source: Qt.resolvedUrl("icons/edit.svg")
                onClicked: stack.push(editSongPage, {"song": del.song})
            }
        }
    }

    Label {
        visible: songList.count === 0
        text: qsTr("Nothing to practice")
        font.italic: true
        anchors.centerIn: parent
    }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        icon.source: Qt.resolvedUrl("icons/add.svg")
        onClicked: stack.push(addSongPage)
    }
}
