import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: logPage
    visible: true
    minimumWidth: 300
    minimumHeight: 400
    maximumWidth: 300
    maximumHeight: 400
    title: qsTr("远程控制")
    color: "white"



    //验证是否是正确ip 格式xx.xxx.xxx.xxx
    function validateIP(ip) {
        var regex = /^([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])$/
        return regex.test(ip)
    }

    //验证是否是正确port 格式1-5位数
    function validatePort(port) {
        var regex = /^([1-9]\d{0,4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-3][0-5])$/
        return regex.test(port)
    }

    //mainPage的content
    ColumnLayout {
        anchors.centerIn: parent
        // IP部分
        RowLayout {
            Label {
                Layout.preferredWidth: 40
                Layout.preferredHeight: 30
                text: "IP:"

                font.bold: true
                font.pixelSize: 16

                horizontalAlignment: Qt.AlignHCenter // 水平居中
                verticalAlignment: Qt.AlignVCenter // 垂直居中
            }
            TextField {
                id: editIp
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                background: Rectangle {
                    color: "lightskyblue"
                    radius: 5
                    border.color: "skyblue"
                    border.width: 3
                }

                font.bold: true
                font.pixelSize: 14

                text: "127.0.0.1"
            }
        }

        // Port部分
        RowLayout {
            Label {
                Layout.preferredWidth: 40
                Layout.preferredHeight: 30

                text: "port:"

                font.bold: true
                font.pixelSize: 14

                horizontalAlignment: Qt.AlignHCenter // 水平居中
                verticalAlignment: Qt.AlignVCenter // 垂直居中
            }
            TextField {
                id: editPort
                Layout.preferredWidth: 150
                Layout.preferredHeight: 30
                background: Rectangle {
                    color: "lightskyblue"
                    radius: 5
                    border.color: "skyblue"
                    border.width: 3
                }

                font.bold: true
                font.pixelSize: 14

                text: "10086"
            }
        }

        RowLayout {
            Rectangle {
                Layout.preferredHeight: 30
            }
        }

        // 连接按钮和分享屏幕按钮
        RowLayout {
            //连接按钮
            MyButton {
                id: btnLink
                text: "连接"
                onClicked: {
                    if (validateIP(editIp.text) && validatePort(
                                editPort.text)) {
                        //IP,port格式正确则传入后端
                        console.log("Valid IP address:", editIp.text)
                        console.log("Valid Port:", editPort.text)
                        if (viewbridge.handleLink(editIp.text, editPort.text)) {
                            //连接成功
                            btnUnShare.enabled = false
                            btnShare.enabled = false
                            btnLink.enabled = false
                            mainPage.show()
                            logPage.hide()
                        } else {
                            //连接失败
                            dialogs.connectFailed.open()
                        }
                    } else {
                        dialogs.textError.open()
                    }
                }
            }
        }

        RowLayout {
            spacing: 35
            //共享屏幕按钮
            MyButton {
                id: btnShare
                text: "共享屏幕"
                onClicked: {
                    viewbridge.handlerShare()
                    btnUnShare.enabled = true
                    btnShare.enabled = false
                    btnLink.enabled = false
                }
            }

            //取消分享按钮
            MyButton {
                id: btnUnShare
                enabled: false
                text: "关闭共享"
                onClicked: {
                    viewbridge.handleCloseShare()
                    btnUnShare.enabled = false
                    btnShare.enabled = true
                    btnLink.enabled = true
                }
            }
        }
    }

    Dialogs {
        id: dialogs
    }

    MainPage {
        id: mainPage
        onClosing: {
            viewbridge.handleClientClose()
            logPage.show()
            mainPage.hide()
            btnUnShare.enabled = false
            btnShare.enabled = true
            btnLink.enabled = true
        }
    }

    Connections {
        target: viewbridge
        function onAcceptInfo(info) {
            if (info === true) {
                dialogs.acceptSuccess.open()
            } else {
                dialogs.acceptFailed.open()
                btnUnShare.enabled = false
                btnShare.enabled = true
                btnLink.enabled = true
            }
        }
        function onConnectSeverOver() {
            dialogs.connectOver.open()
            btnUnShare.enabled = false
            btnShare.enabled = true
            btnLink.enabled = true
        }
        function onConnectClientOver() {
            dialogs.connectOver.open()
            logPage.show()
            mainPage.hide()
            btnUnShare.enabled = false
            btnShare.enabled = true
            btnLink.enabled = true
        }
    }
}
