// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

//        ItemDelegate {
//            Layout.fillWidth: true
//            onClicked: settingsStack.push(backupRestorePage)

//            Label {
//                anchors.fill: parent
//                anchors.margins: 10
//                verticalAlignment: Label.AlignVCenter
//                text: qsTr("Backup and restore")
//            }
//        }

        ItemDelegate {
            Layout.fillWidth: true
            onClicked: settingsStack.push(aboutPage)

            Label {
                anchors.fill: parent
                anchors.margins: 10
                verticalAlignment: Label.AlignVCenter
                text: qsTr("About")
            }
        }

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: dailySetSizeRow.height + 20

            RowLayout {
                id: dailySetSizeRow

                spacing: 10
                anchors.fill: parent
                anchors.margins: 10
                height: dailySetSizeSpinbox.height

                Label {
                    text: qsTr("Daily practice set size")
                    Layout.fillWidth: true
                }

                SpinBox {
                    id: dailySetSizeSpinbox

                    from: 1
                    to: 100
                    value: MuzikoSettings.dailySetSize
                    onValueChanged: MuzikoSettings.dailySetSize = value
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
