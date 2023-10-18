// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

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
            text: qsTr("Your songs and settings have been backed up.")
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
            text: qsTr("Are you sure you want to restore? Any existing songs or settings will be lost!")
        }
    }

    Dialog {
        id: restoreConfirmationDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        Label {
            text: qsTr("Your songs and settings have been restored.")
        }
    }
}
