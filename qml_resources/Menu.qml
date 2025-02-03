import QtQuick 2.15;
import QtQuick.Controls 2.15;
import "chatwindow_dir"
Window {


    id:menu
    width: 1400
    height: 800
    visible: true
    title:myuser
        Rectangle{
            color: color_template[5]
            anchors.fill: parent


            radius: 10
                    clip:true
            Row{
                spacing: 20
                anchors.fill: parent
                anchors.margins: 10
                Rectangle{
                    height:parent.height
                    width: 70
                    radius: 10
                    color: color_template[3]
                    Column{
                        anchors.fill:parent
                        anchors.margins:10
                        Rectangle{
                            clip:true
                            width:parent.width
                            height:width
                            anchors.margins:10;
                            radius:10
                            color:color_template[4]

                            Image{
                                source:"file:"+client.EXE_PATH+"/resource/logout_icon.png"
                                anchors.fill:parent
                            }
                            MouseArea{
                                hoverEnabled:true
                                anchors.fill:parent
                                onEntered: {
                                    parent.opacity=0.8
                                }
                                onExited: {
                                    parent.opacity=1
                                }
                                onClicked:{
                                    main_window.visible=true
                                    client.logout()
                                    client.WindowId=0;
                                    menu.visible=false;
                                }

                            }
                        }
                    }
                    Rectangle{
                        clip:true
                        anchors.bottom:parent.bottom
                        anchors.left:parent.left
                        anchors.right:parent.right
                        anchors.margins:10;
                        radius:10
                        color:color_template[5]
                        height:width
                        Image{
                            source:"file:"+client.EXE_PATH+"/resource/setting_icon.png"
                            anchors.fill:parent
                            anchors.margins:5
                        }
                        MouseArea{
                            hoverEnabled:true
                            anchors.fill:parent
                            onEntered: {
                                parent.opacity=0.8
                            }
                            onExited: {
                                parent.opacity=1
                            }
                            onClicked:{
                                var newWindow = Qt.createQmlObject('SettingMenu{}', parent);
                                newWindow.show();
                            }

                        }
                    }

                }

                ScrollView {

                    width: 200
                    height: parent.height
                    clip: true
                    Column{
                        width: 200
                        anchors.fill: parent
                        spacing: 10
                        TextField {
                            color:"black"
                            id: search_clients
                            placeholderText: qsTr("🔍 search")
                            placeholderTextColor: color_template[3]
                            width: parent.width
                            height: 30

                            background: Rectangle {

                                border.color: username_text.enabled ? color_template[4] : "transparent"
                                radius: 13
                                border.width: 2

                            }

                            onAccepted: {
                                client.send_message(username_text.text+" "+password_text.text)
                                myuser=username_text.text

                            }
                        }
                        Repeater{
                            id:clientsss
                            model: {


                                if (search_clients.text==""){
                                    return client.Clients
                                }
                                else {
                                    let showing=[];
                                    for (let i=0;i<client.Clients.length;i++){
                                        if (client.Clients[i].includes(search_clients.text)){
                                            showing.push(client.Clients[i])
                                            console.log(showing)
                                        }
                                    }
                                    return showing
                                }


                            }
                            Clientbutton{
                                textclient: modelData
                            }
                        }
                    }
                }
                Item{
                    width: parent.width-320
                    height: parent.height
                    clip: true
                    Rectangle{
                        id:chat_window_name
                        width: 200
                        height: 50
                        radius: 5
                        color: color_template[5]

                        Text {
                            text: client.Chatid
                            color: "black"
                            font.bold: true
                            font.pointSize: 15
                            anchors.centerIn: parent

                        }

                    }
                    ChatWindow{
                        width: parent.width

                        anchors.bottom: parent.bottom
                        anchors.top: chat_window_name.bottom
                    }

                }

            }
        }

    onClosing: {
        Qt.exit(0)// Ensure the application quits when the window is closed
    }
}
