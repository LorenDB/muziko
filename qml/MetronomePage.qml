// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import dev.lorendb.muziko

Page {
    id: metronomeRoot

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 40

        Label {
            text: qsTr("Metronome")
            font.pixelSize: 32
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Dial {
            id: bpm

            from: 30
            to: 250
            value: 100
            stepSize: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 30
            onPressedChanged: {
                if (pressed)
                {
                    metronomeLoop.tick = 0;
                    metronomeLoop.stop();
                }
                else
                    metronomeLoop.start();
            }

            ColumnLayout {
                anchors.centerIn: bpm.background
                width: Math.min(bpm.width * 0.8, bpm.height * 0.8)
                height: width
                spacing: 10

                Label {
                    font.pixelSize: 22
                    text: qsTr("%1 bpm").arg(bpm.value)
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    ToolButton {
                        Layout.alignment: Qt.AlignCenter
                        icon.source: Qt.resolvedUrl("icons/remove.svg")
                        icon.width: Math.max(playPauseBtn.icon.width / 2, 20)
                        icon.height: Math.max(playPauseBtn.icon.height / 2, 20)
                        onClicked: --bpm.value
                        autoRepeat: true
                    }

                    ToolButton {
                        id: playPauseBtn

                        Layout.alignment: Qt.AlignCenter
                        icon.width: bpm.width / 4 - 10
                        icon.height: bpm.width / 4 - 10
                        icon.source: metronomeLoop.running ? Qt.resolvedUrl("icons/pause.svg") : Qt.resolvedUrl("icons/play.svg")
                        onClicked: {
                            if (metronomeLoop.running)
                                metronomeLoop.stop();
                            else
                                metronomeLoop.start();
                        }
                    }

                    ToolButton {
                        id: addBtn

                        Layout.alignment: Qt.AlignCenter
                        icon.source: Qt.resolvedUrl("icons/add.svg")
                        icon.width: Math.max(playPauseBtn.icon.width / 2, 20)
                        icon.height: Math.max(playPauseBtn.icon.height / 2, 20)
                        onClicked: ++bpm.value
                        autoRepeat: true
                    }
                }
            }
        }

        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignHCenter

            ComboBox {
                id: upperTimeSignature

                model: ListModel {
                    ListElement { value: 2 }
                    ListElement { value: 3 }
                    ListElement { value: 4 }
                    ListElement { value: 5 }
                    ListElement { value: 6 }
                    ListElement { value: 7 }
                    ListElement { value: 8 }
                    ListElement { value: 9 }
                    ListElement { value: 10 }
                    ListElement { value: 11 }
                    ListElement { value: 12 }
                }
                currentIndex: 2
            }

            Label {
                text: "/"
                font.pixelSize: 22
            }

            ComboBox {
                id: lowerTimeSignature

                model: ListModel {
                    ListElement { value: 2 }
                    ListElement { value: 4 }
                    ListElement { value: 8 }
                }
                currentIndex: 1
            }
        }
    }

    Timer {
        id: metronomeLoop

        property int tick: 0

        running: false
        interval: 60000 / (bpm.value * lowerTimeSignature.currentValue / 4)
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if (tick % upperTimeSignature.currentValue == 0)
            {
                tick = 0;
                metronomeSoundHigh.play();
            }
            else
                metronomeSound.play();
            ++tick;
        }
    }

    SoundEffect {
        id: metronomeSound

        source: Qt.resolvedUrl("res/metronome.wav")
    }

    SoundEffect {
        id: metronomeSoundHigh

        source: Qt.resolvedUrl("res/metronome-high.wav")
    }
}
