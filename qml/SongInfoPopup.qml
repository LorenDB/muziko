import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Popup {
    id: popupRoot

    required property Song song

    anchors.centerIn: parent
    modal: true
    width: 400
    height: 300

    ColumnLayout {
        id: layout

        spacing: 10
        anchors.fill: parent

        Label {
            text: song.name
            font.bold: true
            font.pixelSize: 18
        }

        ListView {
            id: linksList

            states: [
                State {
                    name: "readyToAddLink"
                    PropertyChanges {
                        linksList.footer: addLinkBtn
                    }
                },
                State {
                    name: "addingLink"
                    PropertyChanges {
                        linksList.footer: addLinkInput
                    }
                }
            ]
            state: "readyToAddLink"
            model: popupRoot.song.links
            Layout.fillWidth: true
            Layout.fillHeight: true

            delegate: ItemDelegate {
                width: linksList.width
                height: linkRow.implicitHeight + 20
                onClicked: Qt.openUrlExternally(modelData)

                RowLayout {
                    id: linkRow

                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Label {
                        text: modelData
                        font.underline: true
                        color: Material.color(Material.Blue)
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }

                    ToolButton {
                        icon.source: Qt.resolvedUrl("icons/close.svg")
                        onClicked: popupRoot.song.removeLink(modelData)
                    }
                }
            }

            Component {
                id: addLinkBtn

                ItemDelegate {
                    width: linksList.width
                    text: qsTr("Add a link...")
                    icon.source: Qt.resolvedUrl("icons/add.svg")
                    onClicked: linksList.state = "addingLink"
                }
            }

            Component {
                id: addLinkInput

                ItemDelegate {
                    width: linksList.width
                    height: inputRow.implicitHeight + 20

                    RowLayout {
                        id: inputRow

                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        TextField {
                            id: linkInput

                            Layout.fillWidth: true
                            placeholderText: "Link to song"
                            focus: true
                        }

                        ToolButton {
                            icon.source: Qt.resolvedUrl("icons/done.svg")
                            enabled: Muziko.isValidUrl(linkInput.text)
                            onClicked: {
                                popupRoot.song.addLink(linkInput.text)
                                linksList.state = "readyToAddLink"
                            }
                        }

                        ToolButton {
                            icon.source: Qt.resolvedUrl("icons/close.svg")
                            onClicked: linksList.state = "readyToAddLink"
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
