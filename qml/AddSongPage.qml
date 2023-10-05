import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

Page {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            spacing: 10

            Label {
                text: qsTr("Song name")
            }

            Item { Layout.fillWidth: true }

            TextField {
                id: name

                placeholderText: qsTr("Never Gonna Give You Up")
            }
        }

        RowLayout {
            spacing: 10

            Label {
                text: qsTr("Your current skill level")
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "foo"
            }

            Slider {
                id: proficiency

                from: 0
                to: 2
                stepSize: 1
                snapMode: Slider.SnapAlways
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            spacing: 10

            Item { Layout.fillWidth: true }

            Button {
                enabled: name.text !== ""
                text: qsTr("OK")
                onClicked: {
                    Muziko.songs.addSong(name.text, proficiency.value)
                    stack.pop();
                }
            }

            Button {
                text: qsTr("Cancel")
                onClicked: stack.pop()
            }
        }
    }
}
