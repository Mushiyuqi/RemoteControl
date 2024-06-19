import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 640
    height: 480
    minimumWidth: 480
    minimumHeight: 320
    menuBar: MenuBar {
        Menu {
            title: qsTr("Setting")
            MenuItem {
                action: actions.stop
            }

            MenuItem {
                action: actions.quit
            }
        }
        Menu {
            title: qsTr("Help")
            MenuItem {
                action: actions.about
            }
        }
    }

    header: ToolBar {
        id: toolbar
        RowLayout {

            ToolButton {
                action: actions.stop
            }

            ToolButton {
                action: actions.quit
            }
        }
    }

    Actions {
        id: actions
        about.onTriggered: dialogs.about.open()
    }

    Dialogs {
        id: dialogs
    }

    Content {
        id: content
    }
}
