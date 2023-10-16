// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import dev.lorendb.muziko

ApplicationWindow {
    id: rootWindow

    width: 550
    height: 700
    visible: true
    title: "Muziko"
    Material.theme: Material.Dark
    Material.primary: Material.Blue
    Material.accent: Material.Teal

    Component.onCompleted: {
        if (Muziko.songs == null)
            stack.push(addInstrumentPage);
    }

    footer: TabBar {
        id: pageSwitcher

        height: 60
        Material.accent: "#ffffff"
        background: Rectangle {
            gradient: muzikoGradient
        }

        TabButton {
            icon.source: Qt.resolvedUrl("icons/calendar.svg")
            Layout.alignment: Qt.AlignHCenter
        }

        TabButton {
            icon.source: Qt.resolvedUrl("icons/settings.svg")
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Gradient {
        id: muzikoGradient

        orientation: Gradient.Horizontal

        GradientStop { position: 0; color: "#31ba6f" }
        GradientStop { position: 1; color: "#385e9b" }
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

    StackLayout {
        anchors.fill: parent
        currentIndex: pageSwitcher.currentIndex

        StackView {
            id: stack

            Keys.onReleased: event => {
                if (event.key === Qt.Key_Back && StackLayout.isCurrentItem && stack.depth > 1)
                {
                    event.accepted = true;
                    pop();
                }
            }
            initialItem: songsPage
        }

        SettingsPage {
            Keys.onReleased: event => {
                if (event.key === Qt.Key_Back && StackLayout.isCurrentItem)
                {
                    event.accepted = true;
                    pageSwitcher.currentIndex = 0;
                }
            }
        }
    }
}
