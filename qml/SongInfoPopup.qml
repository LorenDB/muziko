import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import dev.lorendb.muziko

Popup {
    id: popupRoot

    required property Song song

    anchors.centerIn: parent
    modal: true
    width: 400
    height: 500

    ColumnLayout {
        id: layout

        spacing: 10
        anchors.fill: parent

        Label {
            text: song.name
            font.bold: true
            font.pixelSize: 22
            Layout.alignment: Qt.AlignCenter
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true
            Layout.margins: 10

            Label {
                text: qsTr("Last practiced")
                Layout.fillWidth: true
            }

            Label {
                text: popupRoot.song.lastPracticedString
            }

            SongCheckbox {
                id: doneBtn

                Layout.alignment: Qt.AlignVCenter
                width: 20
                height: 20
                checked: popupRoot.song.practicedToday
                onCheckedChanged: popupRoot.song.practicedToday = checked
            }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true
            Layout.margins: 10

            Label {
                text: qsTr("Proficiency")
                Layout.fillWidth: true
            }

            Label {
                text: popupRoot.song.proficiencyString
                color: {
                    if (popupRoot.song.proficiency == Song.LowProficiency)
                        return Material.color(Material.Red);
                    else if (popupRoot.song.proficiency == Song.MediumProficiency)
                        return Material.color(Material.Orange);
                    else if (popupRoot.song.proficiency == Song.HighProficiency)
                        return Material.color(Material.Green);
                    else
                        return palette.text;
                }
            }
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
            boundsBehavior: Flickable.OvershootBounds

            header: Label {
                text: qsTr("Links")
                font.bold: true
                font.pixelSize: 18
                width: linksList.width
                horizontalAlignment: Text.AlignHCenter
            }

            delegate: ItemDelegate {
                width: linksList.width
                height: linkRow.implicitHeight + 20
                onClicked: Qt.openUrlExternally(modelData)

                RowLayout {
                    id: linkRow

                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Image {
                        source: "image://favicon/" + modelData
                    }

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
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: height
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
                            onAccepted: setLinkBtn.clicked()
                        }

                        ToolButton {
                            id: setLinkBtn

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

        RowLayout {
            Layout.fillWidth: true

            ToolButton {
                icon.source: Qt.resolvedUrl("icons/edit.svg")
                onClicked: {
                    popupRoot.close();
                    songsStack.push(editSongPage, {"song": popupRoot.song})
                }
                Layout.fillWidth: true
            }

            ToolButton {
                icon.source: Qt.resolvedUrl("icons/delete.svg")
                icon.color: Material.color(Material.Red)
                onClicked: deleteDialog.open()
                Layout.fillWidth: true
            }
        }
    }

    MessageDialog {
        id: deleteDialog

        buttons: MessageDialog.Ok | MessageDialog.Cancel
        text: qsTr("Are you sure you want to delete %1?").arg(popupRoot.song.name)
        onAccepted: {
            Muziko.songs.removeSong(popupRoot.song.name);
            deleteDialog.close();
            popupRoot.close();
        }
    }
}
