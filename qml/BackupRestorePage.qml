// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import dev.lorendb.muziko

Page {
    id: backupRestoreRoot

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: backupFileDialog.open()

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Export everything to file")
            }
        }

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: restoreFileDialog.open()

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Import everything from file")
            }
        }

        Item { Layout.fillHeight: true }
    }

    Dialog {
        id: backupSuccessDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        Label {
            anchors.fill: parent
            text: qsTr("Your songs and settings have been exported.")
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: restoreDialog

        property string fileUrl: ""

        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            MuzikoSettings.restore(fileUrl);
            while (songsStack.depth > 1)
                songsStack.pop();
            close();
            restoreSuccessDialog.open();
        }

        Label {
            anchors.fill: parent
            text: qsTr("Are you sure you want to import these settings? Any existing songs or settings will be lost!")
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: restoreSuccessDialog

        anchors.centerIn: parent
        standardButtons: Dialog.Ok

        Label {
            anchors.fill: parent
            text: qsTr("Your songs and settings have been imported.")
            wrapMode: Text.WordWrap
        }
    }

    FileDialog {
        id: backupFileDialog

        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        currentFile: "muziko-backup.muziko"
        fileMode: FileDialog.SaveFile
        nameFilters: [qsTr("Muziko Backup Files (*.muziko)")]
        defaultSuffix: "muziko"
        onAccepted: {
            var url = selectedFiles[0];
            if (url !== "" && url !== null) {
                MuzikoSettings.backup(url);
                backupSuccessDialog.open();
            }
        }
    }

    FileDialog {
        id: restoreFileDialog

        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        currentFile: "muziko-backup.muziko"
        fileMode: FileDialog.OpenFile
        nameFilters: [qsTr("Muziko Backup Files (*.muziko)")]
        defaultSuffix: "muziko"
        onAccepted: {
            var url = selectedFiles[0];
            if (url !== "" && url !== null) {
                restoreDialog.fileUrl = url;
                restoreDialog.open();
            }
        }
    }
}
