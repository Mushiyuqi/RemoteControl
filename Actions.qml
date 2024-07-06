import QtQuick
import QtQuick.Controls

Item {

    property alias quit: _quit
    property alias about: _about
    property alias stop: _stop

    Action {
        id: _quit
        text: qsTr("&Quit")
        icon.name: "application-exit"
        shortcut: "Ctrl+q"
        onTriggered: Qt.quit()
    }

    Action {
        id: _about
        text: qsTr("&About")
        icon.name: "help-about"
    }

    Action {
        id: _stop
        text: qsTr("disconnect")
    }
}
