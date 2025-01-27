import QtQuick 2.15
import QtQuick.Controls

Button {
    anchors.fill:parent
    property var filename
    text: filename
    font.pointSize: 10
    font.bold: true

    contentItem: Text {
        text: parent.text
        font: parent.font
        opacity: enabled ? 1.0 : 0.3

        color: parent.down ? "gray" : "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: color_template[2]
        opacity: enabled ? 1 : 0.3
        radius: 10

    }
    onClicked: {
        client.get_private_file(filename)

    }
}
