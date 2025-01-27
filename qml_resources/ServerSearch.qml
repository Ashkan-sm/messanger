import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    color: color_template[1]
    anchors.fill: parent
    Rectangle {
        color: color_template[5]
        anchors.fill: parent
        anchors.margins: 70
        radius: 10
        Rectangle {
            id:server_search_window_title
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top:parent.top
            anchors.margins:10
            height:30
            color:"transparent"
            Text {
                text: "Searching for availabe servers..."
                color: color_template[0]
                font.pointSize: 15
                font.bold: true
            }
        }
        Rectangle{

            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top:server_search_window_title.bottom
            anchors.bottom:parent.bottom
            anchors.margins:10
            radius:10
            color:color_template[4]
            ScrollView {

                width: parent.width
                height: parent.height
                clip: true
                Column{
                    anchors.fill: parent
                    spacing: 10
                    Repeater{
                        model: client.ServerList
                        ServerSelectButton{
                            textclient: modelData
                        }
                    }
                }
            }
        }


    }

}

