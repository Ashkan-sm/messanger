import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.folderlistmodel 1.0

Window {
    id:stickerwindow
    width: 300
    height: 600
    visible: true
    Rectangle{
        anchors.fill: parent
        color: "black"
    }

    FolderListModel {
        id: folderModel
        nameFilters: ["*.png"]

        folder: "file://"+client.EXE_PATH+"/resource/stickers"
    }

    Flickable {
        anchors.fill: parent

        // Scroll content inside the Flickable
        contentWidth: grid.width
        contentHeight: grid.height
        GridView{
            id:grid
            width: stickerwindow.width
            height: stickerwindow.height
            cellWidth: 60
            cellHeight: 60

            model:folderModel
            delegate: Button{
                    width: 50
                    height: 50
                    background: Image {
                        source: model.fileUrl
                        anchors.fill: parent

                        z:1
                        Rectangle{
                            id: butn
                            color: "transparent"
                            x:-9
                            y:-9
                            width: parent.width +18
                            height: parent.height +18
                            radius: 25

                            z:-1
                        }
                    }


                    onHoveredChanged: {

                        if (butn.color===Qt.color("transparent")){
                            butn.color="gray"
                        }
                        else{
                            butn.color="transparent"
                        }
                    }
                    onClicked: {
                        writemsg.text+="<img src='"+model.fileUrl+"' width='25' height='25'>"
                    }

                }


        }
    }
}

