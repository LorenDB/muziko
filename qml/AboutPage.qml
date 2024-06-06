// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import dev.lorendb.muziko

Page {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Image {
            source: Qt.resolvedUrl("icons/logo.svg")
            Layout.preferredWidth: 128
            Layout.preferredHeight: 128
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: qsTr("Muziko")
            font.pixelSize: 28
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
        }

        Label {
            text: qsTr("Version %1").arg(Muziko.muzikoVersion())
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
        }

        Label {
            text: qsTr("Copyright © 2023 Loren Burkholder")
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
        }

        Label {
            text: qsTr("Licensed under the [GNU General Public License version 3](https://github.com/LorenDB/muziko/blob/master/COPYING)")
            textFormat: Label.MarkdownText
            onLinkActivated: link => Qt.openUrlExternally(link)
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
            wrapMode: Label.WordWrap
        }

        Button {
            text: qsTr("View source code on GitHub")
            Layout.alignment: Qt.AlignHCenter
            onClicked: Qt.openUrlExternally("https://github.com/LorenDB/muziko")
            icon.source: Qt.resolvedUrl("icons/external/github.svg")
            icon.color: "transparent"
        }

        Label {
            text: qsTr("Donate")
            font.pixelSize: 20
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
        }

        Label {
            text: qsTr("If you enjoy Muziko and want to show your support, you can donate to the project.")
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
            wrapMode: Label.WordWrap
        }

        Button {
            text: qsTr("Donate with Liberapay")
            Layout.alignment: Qt.AlignHCenter
            onClicked: Qt.openUrlExternally("https://liberapay.com/LorenDB/donate")
            icon.source: Qt.resolvedUrl("icons/external/liberapay.svg")
            icon.color: "transparent"
        }

        Button {
            text: qsTr("Donate with PayPal")
            Layout.alignment: Qt.AlignHCenter
            onClicked: Qt.openUrlExternally("https://www.paypal.com/donate/?business=LSTPU6GJTKCQE&no_recurring=0&item_name=If+you+use+and+enjoy+Muziko%2C+you+can+donate+to+show+your+support+and+to+help+fund+a+port+to+Apple+devices.&currency_code=USD")
            icon.source: Qt.resolvedUrl("icons/external/paypal.svg")
            icon.color: "transparent"
        }

        Item { Layout.fillHeight: true }
    }
}
