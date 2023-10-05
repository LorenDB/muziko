import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import dev.lorendb.muziko

ListView {
    model: Muziko.songs

    delegate: ItemDelegate {
        id: del

        required property string name
        required property date lastPracticed
        required property string proficiency

        width: parent.width

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Label { text: del.name }

            Item { Layout.fillWidth: true }

            Label { text: del.proficiency }
        }
    }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: "+"
        onClicked: stack.push(addSongPage)
    }
}
