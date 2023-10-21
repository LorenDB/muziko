// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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
    minimumWidth: 400
    minimumHeight: 600
    visible: true
    title: "Muziko"
    Material.theme: Material.Dark
    Material.primary: Material.Blue
    Material.accent: Material.Teal

    Component.onCompleted: {
        if (Muziko.songs == null)
            songsStack.push(addInstrumentPage);
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
            icon.source: Qt.resolvedUrl("icons/metronome.svg")
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

    Component {
        id: settingsPage

        SettingsPage {}
    }

    Component {
        id: backupRestorePage

        BackupRestorePage {}
    }

    Component {
        id: aboutPage

        AboutPage {}
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: pageSwitcher.currentIndex

        StackView {
            id: songsStack

            initialItem: songsPage
        }

        MetronomePage {}

        StackView {
            id: settingsStack

            initialItem: settingsPage
        }
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: {
            if (pageSwitcher.currentIndex !== 0)
                return true;
            return songsStack.depth > 1;
        }

        onActivated: {
            switch (pageSwitcher.currentIndex)
            {
            case 0:
                songsStack.pop();
                break;
            case 1:
                if (settingsStack.depth > 1)
                    settingsStack.pop();
                else
                    pageSwitcher.currentIndex = 0;
                break;
            case 2:
                pageSwitcher.currentIndex = 0;
                break;
            default:
                break;
            }
        }
    }
}
