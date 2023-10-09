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

    Component.onCompleted: if (Muziko.songs == null) stack.push(addInstrumentPage)

    header: ToolBar {
        ToolButton {
            id: menuButton

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            icon.source: Qt.resolvedUrl("icons/menu.svg")
            onClicked: drawer.open()
        }

        Label {
            anchors.centerIn: parent
            text: Muziko.songs ? Muziko.songs.instrument : "Muziko"
            horizontalAlignment: Qt.AlignCenter
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

    Drawer {
        id: drawer

        edge: Qt.LeftEdge
        height: parent.height
        width: Math.min(300, rootWindow.width * 0.667)

        ListView {
            anchors.fill: parent
            model: Muziko

            delegate: ItemDelegate {
                required property string name

                text: name
                width: parent.width
                onClicked: {
                    Muziko.setCurrentInstrument(name);
                    drawer.close();
                }
            }

            footer: ItemDelegate {
                text: qsTr("Add...")
                icon.source: Qt.resolvedUrl("icons/add.svg")
                width: parent.width
                onClicked: {
                    drawer.close();
                    stack.push(addInstrumentPage);
                }
            }
        }
    }

    StackView {
        id: stack

        anchors.fill: parent
        initialItem: songsPage
    }
}
