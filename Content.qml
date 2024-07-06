import QtQuick

Item {
    id: root
    anchors.fill: parent
    property alias image: _image
    signal upDateSize
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
                    switch(button){
                        case Qt.LeftButton:{
                            viewbridge.mouseLeftReleaseEvent(eventPoint.position.x, eventPoint.position.y, root.width, root.height)
                            break
                        }
                        case Qt.RightButton:{
                            viewbridge.mouseRightReleaseEvent(eventPoint.position.x, eventPoint.position.y, root.width, root.height)
                            break
                        }
                    }
                  }
        onDoubleTapped: (eventPoint, button)=>{
                            switch(button){
                                case Qt.LeftButton:{

                                    break
                                }
                                case Qt.RightButton:{

                                    break
                                }
                            }
                        }
    }
    HoverHandler{
        onPointChanged: {
            viewbridge.mouseMoveEvent(point.position.x, point.position.y, root.width, root.height)
        }
    }
}
