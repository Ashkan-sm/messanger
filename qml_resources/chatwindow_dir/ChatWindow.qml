import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs



Rectangle{

    property var filesource
    id: chatwindow
    visible: true
    radius: 10
    color:color_template[4]

    Column{

        id :massage_view
        spacing: 10
        anchors.fill: parent
        anchors.margins: 20

        Flickable {

            id:msg_scroll_view
            width: parent.width
            height: parent.height-input_txt_box.height-10

            contentHeight: mssgs.height

            clip: true



            // Column inside Flickable
            Column {
                width: massage_view.width-10
                spacing: 12
                id:mssgs

                Repeater{
                    Component.onCompleted:{
                        console.log(client.Chatid);
                    }
                model: {
                        if (client.Chatid in client.Chats) {
                            return client.Chats[client.Chatid].split("EOMSG").slice(0, -1)
                        }
                    }


                    delegate: Textbox {
                        textdata:modelData
                    }
                    onModelChanged: {
                        msg_scroll_view.contentY = msg_scroll_view.contentHeight-msg_scroll_view.height;
                    }

                }
            }
        }


        Rectangle{
            id:input_txt_box
            height:30
            width: parent.width-30
            radius: 10
            border.color: writemsg.activeFocus ? color_template[0] : color_template[5]
            TextEdit{
                y:5
                x:10
                id:writemsg
                width: parent.width
                height: parent.height
                color: "black"
                text: ""
                textFormat: Text.RichText
                wrapMode: TextEdit.Wrap

                Keys.onReturnPressed: {
                    // scrolltext.ScrollBar.vertical.position = scrolltext.contentHeight
                    client.appendChats(client.Chatid,myuser+"> "+text+"/time "+client.now_time()+"/status 0"+"EOMSG")

                    client.send_private_message(client.Chatid,text+"/time "+client.now_time()+"/status 0")
                    text=""
                }
            }



            Button {

                text: "🙂"
                width: 30
                height: 30
                font.pointSize: 15
                font.bold: true
                anchors.left: parent.right
                anchors.margins: 10
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
                    var newWindow = Qt.createQmlObject('StickerWindow{}', parent);
                    newWindow.show();
                }
            }

            Button {

                text: "📎"
                width: 30
                height: 30
                y:-30
                font.pointSize: 10
                font.bold: true
                anchors.left: parent.right
                anchors.margins: 10
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
                    color: color_template[1]
                    opacity: enabled ? 1 : 0.3
                    radius: 10
                }
                onClicked: {
                    myDialog.open()
                }
            }


        }
    }

    FileDialog{
        id:myDialog
        currentFolder: "/"
        onAccepted: {
            client.send_private_file(client.Chatid,selectedFile)
            client.appendChats(client.Chatid,myuser+"> "+"/file "+selectedFile.toString().substring(selectedFile.toString().lastIndexOf('/')+1)+"/time "+client.now_time()+"/status 0"+"EOMSG")
            client.send_private_message(client.Chatid,"/file "+selectedFile.toString().substring(selectedFile.toString().lastIndexOf('/')+1)+"/time "+client.now_time()+"/status 0")

        }
    }


}


