import QtQuick 2.15

Rectangle{
    color: color_template[3]
    focus: true
    height: 50
    width: parent.width
    radius: 10
    border.color: color_template[4]
    border.width: 3
    property string textclient:"defult"
    Text {
        id: ctext
        text: textclient
        color: "black"
        font.pixelSize: 20
        y: 10
        x:20

    }
    MouseArea{
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            parent.color=color_template[1]
            ctext.color="white"
        }
        onExited: {
            parent.color=color_template[3]
            ctext.color="black"
        }
        onClicked: {
            client.go_to_private(textclient)
            client.Chatid=textclient

        }
    }
    Rectangle{
        visible:client.Notif[textclient]
        color: color_template[5]
        anchors.right: parent.right
        anchors.margins: 10
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height -20
        width: height
        radius: height/2
        Text {

            text: client.Notif[textclient]
            font.bold: true
            anchors.centerIn: parent
            color: color_template[0]
            font.pixelSize: 20
        }
    }



}
