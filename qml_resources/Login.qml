import QtQuick 2.15
import QtQuick.Controls

Rectangle{
    color: color_template[1]
    anchors.fill: parent
    Rectangle{
        id:login_window
        color: color_template[5]
        anchors.fill: parent
        anchors.margins:70
        radius: 10
        Column{
            anchors.margins: 30
            anchors.fill: parent
            spacing: 15
            Text {
                text: "Log in"
                color: color_template[0]
                font.pointSize: 30
                font.bold: true


            }
            TextField {
                color: "black"
                id: username_text
                focus: true
                placeholderText: qsTr("Useename")
                placeholderTextColor: color_template[4]
                width: parent.width
                height: 50
                background: Rectangle {

                    border.color: username_text.enabled ? color_template[5] : "transparent"
                    radius: 5
                    border.width: 2

                }
                onAccepted: {
                    password_text.focus=true

                }
            }

            TextField {
                color:"black"
                echoMode: TextInput.Password
                id: password_text
                placeholderText: qsTr("Password")
                placeholderTextColor: color_template[4]
                width: parent.width
                height: 50

                background: Rectangle {

                    border.color: username_text.enabled ? color_template[5] : "transparent"
                    radius: 5
                    border.width: 2

                }
                onAccepted: {
                    client.send_message_login(username_text.text,password_text.text)
                    myuser=username_text.text

                }
            }

            Button {

                text: qsTr("Log in")
                width: parent.width
                height: 50
                font.pointSize: 15
                font.bold: true
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3

                    color: parent.down ? color_template[4] : color_template[5]
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: color_template[3]
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3

                    radius: 5
                }
                onClicked: {
                    client.send_message_login(username_text.text,password_text.text)
                    myuser=username_text.text
                }
            }

            Button {

                text: qsTr("or, sign up")
                width: parent.width
                height: 50
                font.pointSize: 13
                font.bold: true
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3

                    color: parent.down ? "#CCCCCC" : "#535353"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: color_template[5]
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3

                    radius: 5
                }
                onClicked: {
                    login_window.visible=false
                    sign_up_window.visible=true

                }
            }
        }
    }


    Rectangle{
        visible:false
        id:sign_up_window
        color: color_template[5]
        anchors.fill: parent
        anchors.margins:70
        radius: 10
        Column{
            anchors.margins: 30
            anchors.fill: parent
            spacing: 15
            Text {
                text: "Sign up"
                color: color_template[0]
                font.pointSize: 30
                font.bold: true


            }
            TextField {
                color: "black"
                id: username_text_signup
                placeholderText: qsTr("Useename")
                placeholderTextColor: color_template[4]
                width: parent.width
                height: 50
                background: Rectangle {

                    border.color: username_text_signup.enabled ? color_template[5] : "transparent"
                    radius: 5
                    border.width: 2

                }
                onAccepted: {
                    password_text_signup.focus=true

                }
            }

            TextField {
                color:"black"
                echoMode: TextInput.Password
                id: password_text_signup
                placeholderText: qsTr("Password")
                placeholderTextColor: color_template[4]
                width: parent.width
                height: 50

                background: Rectangle {

                    border.color: username_text_signup.enabled ? color_template[5] : "transparent"
                    radius: 5
                    border.width: 2

                }
                onAccepted: {
                    client.send_message_signup(username_text_signup.text,password_text_signup.text)
                    myuser=username_text_signup.text

                }
            }

            Button {

                text: qsTr("Sign up")
                width: parent.width
                height: 50
                font.pointSize: 15
                font.bold: true
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3

                    color: parent.down ? color_template[4] : color_template[5]
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: color_template[3]
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3

                    radius: 5
                }
                onClicked: {

                    client.send_message_signup(username_text_signup.text,password_text_signup.text)
                    myuser=username_text_signup.text
                }
            }

            Button {

                text: qsTr("or, login")
                width: parent.width
                height: 50
                font.pointSize: 13
                font.bold: true
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3

                    color: parent.down ? "#CCCCCC" : "#535353"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: color_template[5]
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3

                    radius: 5
                }
                onClicked: {
                    login_window.visible=true
                    sign_up_window.visible=false

                }
            }
        }
    }


    Component.onCompleted: {
        client.WindowIdChanged.connect(function() {
            if (client.WindowId===1){
                main_window.visible=false
                var newWindow = Qt.createQmlObject('Menu {}', parent);
                newWindow.show();

            }
        });
    }


}

