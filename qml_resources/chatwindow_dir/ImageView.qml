import QtQuick 2.15

Image{
    property var image_data:""
    z:-1
    id:img_it_self
    anchors.fill:parent

    source:{
        if (visible) {
            var index = textdata.indexOf("/image");
            return "file:/"+client.EXE_PATH + "/savedata_client/" + textdata.substring(index + 7, textdata.indexOf("/time"))
        }
    }

    Rectangle{
        radius:20
        color:"transparent"
        border.color:color_template[4];
        border.width:radius/2

        x:-radius/2
        y:-radius/2
        width:parent.width+radius
        height:parent.height+radius

    }


}
