import QtQuick 2.12

Rectangle {
    id: root
    width: 150
    height: 35
    radius: 6
    color: hovered ? Qt.darker("#49b6d7", 1.18) : "#49b6d7"
    border.color: Qt.darker("#49b6d7", 1.18)
    border.width: 1
    property alias text: text.text
    property alias fontSize: text.font.pointSize
    property bool hovered: false
    property int widthMargin: 10
    property int heightMargin: 6
    property color textColor: text.color

    signal clicked
    signal pressed
    signal released
    signal entered
    signal exited

    Text {
        id: text
        x: widthMargin
        y: heightMargin
        horizontalAlignment: Qt.AlignHCenter // 水平居中
        verticalAlignment: Qt.AlignVCenter // 垂直居中
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            root.pressed()
            text.x += 1
            text.y += 1
        }
        onReleased: {
            root.released()
            root.clicked()
            text.x -= 1
            text.y -= 1
        }
        onEntered: root.hovered = true
        onExited: root.hovered = false
    }
    states: [
        State {
            name: "disable"
            when: !enabled
            PropertyChanges {
                root.color: Qt.lighter("gray", 1.18)
                text.color: "lightgray"
                root.border.color: "gray"
            }
        }
    ]
}
