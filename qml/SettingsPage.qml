// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

       ItemDelegate {
           Layout.fillWidth: true
           onClicked: settingsStack.push(backupRestorePage)

           Label {
               anchors.fill: parent
               anchors.margins: 10
               verticalAlignment: Label.AlignVCenter
               text: qsTr("Import/export all settings")
           }
       }

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

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: useAmoledThemeRow.height + 20

            RowLayout {
                id: useAmoledThemeRow

                spacing: 10
                anchors.fill: parent
                anchors.margins: 10
                height: useAmoledThemeSwitch.height

                Label {
                    text: qsTr("Use AMOLED theme")
                    Layout.fillWidth: true
                }

                Switch {
                    id: useAmoledThemeSwitch

                    checked: MuzikoSettings.useAmoledTheme
                    onCheckedChanged: MuzikoSettings.useAmoledTheme = checked
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
