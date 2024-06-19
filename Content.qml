import QtQuick

Item {
    anchors.fill: parent
    property alias image: _image
    Image {
        id: _image
        anchors.fill: parent
        source: "image://img"
        fillMode:Image.PreserveAspectFit
        cache: false
    }
    Connections {
        target: viewbridge
        onNeedUpdate: {
            console.log("qml端重载图片")
            image.source = "image://img/" + Math.random()
        }
    }
}
