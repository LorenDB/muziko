// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    id: backupRestoreRoot

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: {
                MuzikoSettings.backup();
                backupDialog.open();
            }

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Backup songs and settings")
            }
        }

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: restoreDialog.open()

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Restore songs and settings")
            }
        }

        Item { Layout.fillHeight: true }
    }

    Dialog {
        id: backupDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        Label {
            anchors.fill: parent
            text: qsTr("Your songs and settings have been backed up.")
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: restoreDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            MuzikoSettings.restore();
            while (songsStack.depth > 1)
                songsStack.pop();
            close();
            restoreConfirmationDialog.open();
        }

        Label {
            anchors.fill: parent
            text: qsTr("Are you sure you want to restore? Any existing songs or settings will be lost!")
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: restoreConfirmationDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        Label {
            anchors.fill: parent
            text: qsTr("Your songs and settings have been restored.")
            wrapMode: Text.WordWrap
        }
    }
}
