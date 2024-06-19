import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    minimumWidth: viewbridge.getImgWidth()
    maximumWidth: viewbridge.getImgWidth()
    minimumHeight: viewbridge.getImgHeight()
    maximumHeight: viewbridge.getImgHeight()

    Dialogs {
        id: dialogs
    }

    Content {
        id: content
    }
}
