import QtQuick 2.12

Rectangle {
    id: root
    width: text.width + widthMargin * 2
    height: text.height + heightMargin * 2
    radius: 6
    color: hovered ? Qt.darker("lightskyblue", 1.18) : "lightskyblue"
    border.color: Qt.darker("lightskyblue", 1.18)
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
