import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    minimumWidth: viewbridge.getImgWidth() * 0.4
    minimumHeight: viewbridge.getImgHeight() * 0.4
    maximumWidth: viewbridge.getImgWidth() * 0.4
    maximumHeight: viewbridge.getImgHeight() * 0.4

    Dialogs {
        id: dialogs
    }

    Content {
        id: content
        function onUpDateSize(){
            root.minimumWidth = viewbridge.getImgWidth() * 0.4
            root.minimumHeight = viewbridge.getImgHeight() * 0.4
            root.maximumWidth = viewbridge.getImgWidth() * 0.4
            root.maximumHeight = viewbridge.getImgHeight() * 0.4
        }
    }
}
