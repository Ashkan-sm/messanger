import QtQuick 2.15
import QtQuick.Controls
import Qt.labs.folderlistmodel 1.0

Window {


    id:setting_window
    width: 600
    height: 400
    visible: true

    ScrollView {
        anchors.fill:parent
        clip: true
        Column {
            spacing: 10
            Repeater {
                model: colorlist
                Row {
                    Repeater {
                        id: lol
                        model: modelData

                        Rectangle {
                            height: 50
                            width: 100
                            color: modelData
                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                onEntered: {
                                    parent.parent.opacity = 0.8
                                }
                                onExited: {
                                    parent.parent.opacity = 1
                                }
                                onClicked: {
                                    color_template = lol.model
                                }

                            }

                        }

                    }

                }

            }
        }
    }
}