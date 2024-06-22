import QtQuick
import QtQuick.Controls

Button {
    id: root

    background: Rectangle {
        id: rect
        radius: 6
        color: hovered ? Qt.darker("#49b6d7", 1.18) : "#49b6d7"
        border.color: Qt.darker("#49b6d7", 1.18)
        border.width: 1
    }

    //凹陷弹出效果
    onPressed: {
        x += 1
        y += 1
    }
    onReleased: {
        x -= 1
        y -= 1
    }

    states: [
        State {
            //禁用状态
            name: "disable"
            when: !enabled
            PropertyChanges {
                rect.color: Qt.lighter("gray", 1.18)
                // text.color: "lightgray"
                rect.border.color: "gray"
            }
        }
    ]
}
