import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import "qml_resources"


ApplicationWindow {
    id:main_window
    width: 640
    height: 520
    visible: true
    title: qsTr("Ashkan_ChatRoom")


    property string myuser:"defult"
    property var colorlist:[
        ["#0a2344","#3b1d4a","#6b1650","#9c1057","#cc095d","#fd0363"],
        ["#23272a","#2c2f33","#7289da","#92a1b3","#99aab5","#ffffff"],
        ["#143601","#1a4301","#245501","#538d22","#73a942","#aad576"],
        ["#023e8a","#0077b6","#0096c7","#00b4d8","#48cae4","#90e0ef"],
        ["#000000","#333333","#666666","#999999","#cccccc","#ffffff"],
        ["#00132d","#00193b","#001e45","#002657","#002d67","#00377e"],
        ["#006d77","#83c5be","#e29578","#e8aa93","#ffddd2","#edf6f9"],
        ["#27272b","#6a30fe","#9f4cfd","#e16be6","#fb89cc","#dfdcdf"],
        ["#5c8148","#76a45b","#8bb650","#a1c181","#b3e093","#d6ec89"],
        ["#010b12","#1e1f21","#0c8900","#2bc20e","#9cff00","#39ff13"],
        ["#10002b","#240046","#3c096c","#5a189a","#7b2cbf","#9d4edd"],
    ]
    property var color_template:colorlist[1]
    ServerSearch{
        id:search_server_window
        visible: true
    }
    Login{
        id:login_window
        visible: false
    }

}
