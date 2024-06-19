import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    minimumWidth: viewbridge.getImgWidth()
    maximumWidth: viewbridge.getImgWidth()
    minimumHeight: viewbridge.getImgHeight()
    maximumHeight: viewbridge.getImgHeight()

    // menuBar: MenuBar {
    //     Menu {
    //         title: qsTr("Setting")
    //         MenuItem {
    //             action: actions.stop
    //         }

    //         MenuItem {
    //             action: actions.quit
    //         }
    //     }
    //     Menu {
    //         title: qsTr("Help")
    //         MenuItem {
    //             action: actions.about
    //         }
    //     }
    // }

    // header: ToolBar {
    //     id: toolbar
    //     RowLayout {

    //         ToolButton {
    //             action: actions.stop
    //         }

    //         ToolButton {
    //             action: actions.quit
    //         }
    //     }
    // }

    // Actions {
    //     id: actions
    //     about.onTriggered: dialogs.about.open()
    // }
    Dialogs {
        id: dialogs
    }

    Content {
        id: content
    }
    // Component.onCompleted: {
    //     root.width = viewbridge.getImgWidth()
    //     root.height = viewbridge.getImgHeight()
    // }
}
