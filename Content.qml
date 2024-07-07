import QtQuick

Item {
    id: root
    anchors.fill: parent
    property alias image: _image
    signal upDateSize
    focus: true
    Image {
        id: _image
        anchors.fill: parent
        source: "image://img/"

        fillMode: Image.PreserveAspectFit
        cache: false
    }
    Connections {
        target: viewbridge
        function onNeedUpdate() {
            console.log("qml端重载图片")
            _image.source = "image://img/" + Math.random()
            root.upDateSize()
        }
    }

    //鼠标
    TapHandler{
        onTapped: (eventPoint, button)=>{
                        console.log("mouse on tapped")
                        viewbridge.mouseTappedEvent(eventPoint.position.x, eventPoint.position.y, root.width, root.height, button)
                    }
        onDoubleTapped: (eventPoint)=>{
                        console.log("mouse on double tapped")
                        viewbridge.mouseDoubleTappedEvent(eventPoint.position.x, eventPoint.position.y, root.width, root.height)
                    }
    }
    // HoverHandler{
    //     onPointChanged: {
    //         viewbridge.mouseMoveEvent(point.position.x, point.position.y, root.width, root.height)
    //     }
    // }
    WheelHandler {
        onWheel: (wheel) => {
            viewbridge.mouseWheelEvent(wheel.position.x, wheel.position.y, root.width, root.height, wheel.angleDelta.y);
        }
    }
    // 键盘事件处理器
    Keys.onPressed: (event) => {
        console.log("Keys onPressed")
        viewbridge.keyPressEvent(event.key);
    }

    Keys.onReleased: (event) => {
        console.log("Keys onReleased")
        viewbridge.keyReleaseEvent(event.key);
    }

}
