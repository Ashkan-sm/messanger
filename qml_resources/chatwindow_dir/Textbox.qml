import QtQuick 2.15

Item{
    property string textdata: "defult"

    Component.onCompleted:{
        var usermsg=textdata.substring(0,textdata.indexOf(">"));
        myuser === usermsg ? anchors.right = parent.right : anchors.left = parent.left
        // color: myuser === textdata.substring(0,textdata.indexOf(">")) ? color_template[2] : color_template[5]
        if (textdata.indexOf("/file")>=0){
            width= textItem.implicitWidth + 50
            height= textItem.implicitHeight
            Qt.createQmlObject('Download_button{        id:d_buttton\n' +
                '        filename:{return textdata.substring(textdata.lastIndexOf("/file")+6,textdata.indexOf("/time"))}}',this);


        }
        else if(textdata.indexOf("/image")>=0){
            width= 400
            height= 300
            Qt.createQmlObject('ImageView{        id:img_view\n' +
                '        image_data:textdata}',this);
        }
        else if(textdata.indexOf("/video")>=0){
            width= 400
            height= 300
            Qt.createQmlObject('    VideoView{id:video_view\n' +
                'video_data:textdata}',this);
        }
        else
        {

            width= textItem.implicitWidth + 50
            height= textItem.implicitHeight
            tboxobj.visible=true;

        }
    }
    Rectangle {
        visible:false
        id: tboxobj
        color: myuser === textdata.substring(0,textdata.indexOf(">")) ? color_template[2] : color_template[5]
        anchors.fill:parent
        radius: height / 2

        Text {
            y: -height / 2 + font.pixelSize / 2
            x: 10
            id: textItem
            color: myuser ===textdata.substring(0,textdata.indexOf(">")) ? color_template[5] : color_template[0]
            font.pixelSize: 15
            font.bold: true


            text: {
                var index = textdata.indexOf(">");
                return textdata.substring(index + 1, textdata.indexOf("/time"))
            }

            textFormat: Text.RichText


        }
        Rectangle {
            width: 20
            height: 10
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            color: "transparent"

            Text {
                color: "white"
                text: {
                    var state = textdata.substring(textdata.indexOf("/status") + 8, textdata.indexOf("/status") + 9)
                    if (state == "0") {
                        return "..."
                    } else if (state == "1") {
                        return "✔"
                    } else if (state == "2") {
                        return "✔✔"
                    } else if (state == "3") {
                        color = color_template[4]
                        return "✔✔"
                    }
                    return state;
                }

                anchors.fill: parent
            }
        }
        Rectangle {
            width: 50
            height: 15
            anchors.left: parent.left
            y: parent.height - 2
            anchors.leftMargin: 10
            color: "transparent"

            Text {
                color: "black"
                text: {textdata.substring(textdata.indexOf("/time") + 5, textdata.indexOf("+"))}
                anchors.fill: parent
                font.pointSize: parent.height / 2
            }
        }

    }

}
