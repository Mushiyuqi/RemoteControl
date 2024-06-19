import QtQuick

Item {
    anchors.fill: parent
    Image {
        id: image
        anchors.fill: parent
        source: "image://img"
        fillMode: Image.Stretch
        cache: false
    }
    Connections {
        target: controller
        onNeedUpdate: {
            console.log("qml端重载图片")
            image.source = "image://img/" + Math.random()
        }
    }
}
