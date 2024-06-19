import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {

    property alias about: _about
    property alias textError: _textError
    MessageDialog {
        id: _about
        modality: Qt.WindowModal
        buttons: MessageDialog.Ok
        text: "A RemoteControl APP"
        informativeText: qsTr("you can link to another computer and control it or being controlled")
        detailedText: "Copyright©2024  group 2"
    }

    MessageDialog {
        id: _textError
        buttons: MessageDialog.Ok
        text: "IP或Port输出错误"
        informativeText: "控制他人则输入他人ip,分享屏幕则输入本人ip"
    }
}
