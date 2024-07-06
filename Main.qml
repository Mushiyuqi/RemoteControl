import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import Qt.labs.platform

Window {

    id: logPage
    visible: true
    minimumWidth: 600
    minimumHeight: 400
    maximumWidth: 600
    maximumHeight: 400
    title: qsTr("远程控制")

    // 验证是否是正确ip 格式xx.xxx.xxx.xxx
    function validateIP(ip) {
        var regex = /^([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])$/
        return regex.test(ip)
    }
    // 验证是否是正确port 格式1-5位数
    function validatePort(port) {
        var regex = /^([1-9]\d{0,4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-3][0-5])$/
        return regex.test(port)
    }
    Rectangle {
        //外背景
        id: background
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#0fa1d8"
            }
            GradientStop {
                position: 0.2
                color: "#49b6d7"
            }
            GradientStop {
                position: 0.66
                color: "#81d2d8"
            }
            GradientStop {
                position: 1.0
                color: "#49b6d7"
            }
        }
    }

    RectangularGlow {
        //发光边缘
        id: effect
        anchors.fill: rect
        glowRadius: 10
        spread: 0.2
        color: "white"
        cornerRadius: rect.radius + glowRadius
    }

    Rectangle {
        //内白板
        id: rect
        color: "white"
        anchors.centerIn: parent
        width: Math.round(parent.width / 1.3)
        height: Math.round(parent.height / 1.4)
        radius: 25
        opacity: 0.9
        //内容
        ColumnLayout {
            //左半部分
            id: leftlayout
            RowLayout {
                //app标题
                spacing: 20
                Layout.margins: 20
                Image {
                    //logo
                    id: applogo
                    Layout.preferredWidth: 70
                    Layout.preferredHeight: 70
                    source: "qrc:/logo/communicate.png"
                    fillMode: Image.PreserveAspectCrop
                    smooth: true
                }
                Text {
                    //title
                    id: apptitle
                    Layout.preferredWidth: text.width + 20
                    Layout.preferredHeight: text.height + 20
                    text: qsTr("远程控制")
                    font.family: "KaiTi"
                    font.pixelSize: 25
                }
            }
            Label {
                //IP部分
                Layout.preferredWidth: 40
                Layout.leftMargin: 17
                Layout.topMargin: 10
                text: "IP:"
                font.bold: true
                font.pixelSize: 16
            }
            RowLayout {
                Image {
                    //iplogo
                    Layout.leftMargin: 16
                    Layout.rightMargin: 1
                    fillMode: Image.PreserveAspectFit

                    source: "qrc:/logo/video.png"
                }
                TextField {
                    //ip输入框
                    id: editIp
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    background: Rectangle {
                        color: "#49b6d7"
                        radius: 5
                        border.color: Qt.darker("#49b6d7", 1.18)
                        border.width: 2
                    }

                    font.bold: true
                    font.pixelSize: 14

                    text: "127.0.0.1"
                }
            }
            // Port部分
            Label {
                Layout.preferredWidth: 40
                Layout.preferredHeight: 30
                Layout.leftMargin: 17
                text: "port:"
                font.bold: true
                font.pixelSize: 14

                horizontalAlignment: Qt.AlignHCenter // 水平居中
                verticalAlignment: Qt.AlignVCenter // 垂直居中
            }
            RowLayout {
                Image {
                    //port图标
                    Layout.leftMargin: 16
                    Layout.rightMargin: 1
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Qt.AlignHCenter // 水平居中
                    verticalAlignment: Qt.AlignVCenter // 垂直居中
                    source: "qrc:/logo/video.png"
                }
                TextField {
                    //port输入框
                    id: editPort
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 30
                    background: Rectangle {
                        color: "#49b6d7"
                        radius: 5
                        border.color: Qt.darker("#49b6d7", 1.18)
                        border.width: 2
                    }

                    font.bold: true
                    font.pixelSize: 14

                    text: "10086"
                }
            }
        }

        ColumnLayout {
            //右半部分
            id: rightlayout
            anchors.left: leftlayout.right
            //连接按钮
            MyButton {
                id: btnLink
                text: "连 接"
                Layout.margins: 30
                Layout.preferredWidth: 150
                Layout.preferredHeight: 35
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
            Rectangle {
                //分隔
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                Layout.leftMargin: 30
                Layout.rightMargin: 30
                Layout.preferredWidth: 150
                height: 1
                color: "lightgray"
            }

            //共享屏幕按钮
            MyButton {
                id: btnShare
                Layout.topMargin: 30
                Layout.bottomMargin: 10
                Layout.leftMargin: 30
                Layout.preferredWidth: 150
                Layout.preferredHeight: 35
                text: "共享屏幕"
                onClicked: {
                    if (validateIP(editIp.text) && validatePort(
                                editPort.text)) {
                        viewbridge.handlerShare()
                        btnUnShare.enabled = true
                        btnShare.enabled = false
                        btnLink.enabled = false
                    } else {
                        dialogs.textError.open()
                    }
                }
            }

            //关闭共享按钮
            MyButton {
                id: btnUnShare
                Layout.rightMargin: 30
                Layout.topMargin: 10
                Layout.leftMargin: 30
                Layout.preferredWidth: 150
                Layout.preferredHeight: 35
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
    //系统托盘
    SystemTrayIcon {
        visible: true
        icon.source: "qrc:/logo/communicate.png"
        onActivated: {
            if (logPage.visible === true && mainPage.visible === false) {
                logPage.show()
                logPage.raise()
                logPage.requestActivate()
            } else if (logPage.visible === false && mainPage.visible === true) {
                mainPage.show()
                mainPage.raise()
                mainPage.requestActivate()
            }
        }
    }
}
