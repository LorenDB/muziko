// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import dev.lorendb.muziko

ApplicationWindow {
    id: rootWindow

    width: 640
    height: 480
    visible: true
    title: "Muziko"
    Material.theme: Material.Dark
    Material.primary: Material.Blue
    Material.accent: Material.Cyan

    Component.onCompleted: {
        if (Muziko.songs == null)
            stack.push(addInstrumentPage);

        // this back key handling method comes from https://stackoverflow.com/a/67357598/12533859
        contentItem.Keys.released.connect(function(event) {
            if (event.key === Qt.Key_Back && rootStackView.depth > 1) {
                event.accepted = true
                rootStackView.pop()
            }
            else
                event.accepted = false
        })
        // TODO: do same with mouse back button
    }

    header: ToolBar {
        background: Rectangle {
            implicitHeight: 60
            gradient: Gradient {
                orientation: Gradient.Horizontal

                GradientStop { position: 0; color: "#31ba6f" }
                GradientStop { position: 1; color: "#385e9b" }
            }
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
                text: Muziko.songs ? Muziko.songs.instrument : "Muziko"
                horizontalAlignment: Qt.AlignCenter
                padding: 10
            }
        }
    }

    footer: ToolBar {
        background: Rectangle {
            implicitHeight: 60
            gradient: Gradient {
                orientation: Gradient.Horizontal

                GradientStop { position: 0; color: "#31ba6f" }
                GradientStop { position: 1; color: "#385e9b" }
            }
        }

        ListView {
            anchors.fill: parent
            orientation: ListView.Horizontal
        }

        RowLayout {
            anchors.fill: parent

            ToolButton {
                icon.source: Qt.resolvedUrl("icons/calendar.svg")
                Layout.alignment: Qt.AlignHCenter
            }

            ToolButton {
                icon.source: Qt.resolvedUrl("icons/settings.svg")
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    Component {
        id: songsPage

        SongsPage {}
    }

    Component {
        id: addInstrumentPage

        AddInstrumentPage {}
    }

    Component {
        id: addSongPage

        AddSongPage {}
    }

    Component {
        id: editSongPage

        EditSongPage {}
    }

    component InstrumentPicker : Popup {
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
                }
            }

            footer: ItemDelegate {
                text: qsTr("Add...")
                icon.source: Qt.resolvedUrl("icons/add.svg")
                width: parent.width
                onClicked: stack.push(addInstrumentPage)
            }
        }
    }

    InstrumentPicker { id: instrumentPicker }

    StackView {
        id: stack

        anchors.fill: parent
        initialItem: songsPage
    }
}
