import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {

    property alias about: _about
    property alias textError: _textError
    property alias connectOver: _connectOver
    property alias connectFailed: _connectFailed
    property alias acceptSuccess: _acceptSuccess
    property alias acceptFailed: _acceptFailed
    property alias debugDialog: _debugDialog

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
    MessageDialog {
        id: _connectOver
        buttons: MessageDialog.Ok
        text: "连接被断开"

        informativeText: ""
    }
    MessageDialog {
        id: _connectFailed
        buttons: MessageDialog.Ok
        text: "连接失败"
        informativeText: ""
    }
    MessageDialog {
        id: _acceptSuccess
        buttons: MessageDialog.Ok
        text: "共享成功"
        informativeText: ""
    }
    MessageDialog {
        id: _acceptFailed
        buttons: MessageDialog.Ok
        text: "共享失败"
        informativeText: ""
    }
    MessageDialog {
        id: _debugDialog
        buttons: MessageDialog.Ok
        text: "this is debug"
        informativeText: ""
    }
}
