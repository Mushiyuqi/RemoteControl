import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: logPage
    visible: true
    width: 480
    height: 320
    minimumWidth: 480
    minimumHeight: 320
    title: qsTr("远程控制")
    color: "gainsboro"

    property bool connected: false

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
                font.pixelSize: 12

                placeholderText: "127.0.0.1"
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

                placeholderText: "10086"
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
            Button {
                id: btnLink
                Layout.preferredWidth: 80
                Layout.preferredHeight: 30

                background: Rectangle {
                    color: "lightskyblue"
                    radius: 5
                    border.color: "skyblue"
                    border.width: 3
                }

                font.pixelSize: 16

                text: "连接"
                onClicked: {
                    if (validateIP(editIp.text) && validatePort(
                                editPort.text)) {
                        //IP,port格式正确则传入后端
                        console.log("Valid IP address:", editIp.text)
                        console.log("Valid Port:", editPort.text)
                        if (controller.handleValidText(editIp.text,
                                                       editPort.text)) {
                            mainPage.show()
                            logPage.hide()
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
            Button {
                id: btnShare
                Layout.preferredWidth: 80
                Layout.preferredHeight: 30

                background: Rectangle {
                    color: "lightskyblue"
                    radius: 5
                    border.color: "skyblue"
                    border.width: 3
                }

                font.pixelSize: 16

                text: "共享屏幕"
                onClicked: {
                    if (validateIP(editIp.text) && validatePort(
                                editPort.text)) {
                        //IP,port格式正确则传入后端
                        console.log("Valid IP address:", editIp.text)
                        console.log("Valid Port:", editPort.text)
                        if (controller.handleValidText(editIp.text,
                                                       editPort.text)) {
                            btnUnShare.enabled = true
                            btnShare.enabled = false
                            //调用后端逻辑
                        }
                    } else {
                        dialogs.textError.open()
                    }
                }
            }

            //取消分享按钮
            Button {
                id: btnUnShare
                enabled: false
                Layout.preferredWidth: 80
                Layout.preferredHeight: 30

                background: Rectangle {
                    color: "lightskyblue"
                    radius: 5
                    border.color: "skyblue"
                    border.width: 3
                }

                font.pixelSize: 16

                text: "关闭共享"
            }
        }
    }
    Dialogs {
        id: dialogs
    }
    MainPage {
        id: mainPage
    }
}
