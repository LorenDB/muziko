// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    component InstrumentPicker : Popup {
        id: instrumentPickerRoot

        anchors.centerIn: parent
        modal: true
        width: 400
        height: instruments.contentHeight + padding * 2

        ListView {
            id: instruments

            anchors.fill: parent
            model: Muziko

            delegate: ItemDelegate {
                required property string name

                text: name
                width: parent.width
                onClicked: {
                    Muziko.setCurrentInstrument(name);
                    instrumentPickerRoot.close();
                }
            }

            footer: ItemDelegate {
                text: qsTr("Add...")
                icon.source: Qt.resolvedUrl("icons/add.svg")
                width: parent.width
                onClicked: {
                    stack.push(addInstrumentPage);
                    instrumentPickerRoot.close();
                }
            }
        }
    }

    InstrumentPicker { id: instrumentPicker }

    footer: ToolBar {
        background: Rectangle {
            implicitHeight: 60
            gradient: Gradient {
                orientation: Gradient.Horizontal

                GradientStop { position: 0; color: "#31ba6f" }
                GradientStop { position: 1; color: "#385e9b" }
            }
        }

        CheckBox {
            id: showAllCb

            text: qsTr("Show all")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 10
        }

        Rectangle {
            color: "#33ffffff"
            radius: height / 2
            width: Math.max(instrumentLabel.implicitWidth + instrumentLabel.padding * 2, 150)
            height: instrumentLabel.implicitHeight
            border.color: "#ffffff"
            border.width: 1
            anchors.centerIn: parent

            TapHandler {
                onTapped: instrumentPicker.open()
            }

            Label {
                id: instrumentLabel

                anchors.centerIn: parent
                text: Muziko.songs ? Muziko.songs.instrument : ""
                horizontalAlignment: Qt.AlignCenter
                padding: 10
            }
        }

        RoundButton {
            icon.source: Qt.resolvedUrl("icons/add.svg")
            onClicked: stack.push(addSongPage)
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 10
        }
    }

    ListView {
        id: songList

        anchors.fill: parent
        model: showAllCb.checked ? Muziko.songs : Muziko.songsFilter
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
    }
}
