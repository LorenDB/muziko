// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

import QtCore
import QtQuick
import QtQuick.Controls
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
            onClicked: backupFolderPicker.open()

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Backup songs and settings to file")
            }
        }

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: restoreFilePicker.open()

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("Restore from file")
            }
        }

        Item { Layout.fillHeight: true }
    }

    FolderDialog {
        id: backupFolderPicker

        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: MuzikoSettings.backup(selectedFolder)
    }

    FileDialog {
        id: restoreFilePicker

        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            while (songsStack.depth > 1)
                songsStack.pop();
            MuzikoSettings.restore(selectedFile);
        }
    }
}
