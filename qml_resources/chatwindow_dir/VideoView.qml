import QtQuick 2.15
import QtMultimedia 5.15
import QtMultimedia

Video{

    property var video_data:""
    z:-1
    id:videoPlayer
    anchors.fill:parent


    autoPlay:MediaPlayer.Infinite

    source:{
        if (visible) {
            var index = textdata.indexOf("/video");
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
    MouseArea {
        anchors.fill: parent
        onClicked: {
            videoPlayer.play()
        }
    }


    Keys.onSpacePressed: video.playbackState == MediaPlayer.PlayingState ? video.pause() : video.play()
    Keys.onLeftPressed: video.position = video.position - 5000
    Keys.onRightPressed: video.position = video.position + 5000
    Component.onCompleted: {

    }
}
