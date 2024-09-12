import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import HcControls
import Config
import CustomControls
import DataHandler
import OperateLogManager


Rectangle {
    id: root

    implicitWidth: 170
    color: "#484b4c"


    /*******************功能组件/函数*******************/

    //接收动作执行结果
    Connections {
        target: DataHandler

        function onSendActionResult(msg) {
            var dlg = HcMessageDlg.createMessageDlg(mainWindow, msg)
            dlg.onlyConfirm = true
            dlg.open()
        }
    }




    /**********************绘制ui***********************/

    //指令下拉框
    ComboBox {
        id: comboBox
        width: 90
        height: 35
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 3
        model: ["复位", "开盖", "关盖", "发送方案", "开始", "停止"]

        //弹出窗口样式
        popup: Popup {
            width: comboBox.width + 100
            y: comboBox.height

            Column {
                id: column
                anchors.fill: parent
                spacing: 4

                Repeater {
                    model: comboBox.model

                    delegate: RadioButton {
                        text: modelData
                    }
                }

                Row {
                    spacing: 10
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 10

                    Button {
                        id: btnOK
                        text: "确定"
                        onClicked: {
                            for (var i = 0; i < column.children.length; i++) {
                                if (column.children[i].checked) {
                                    var rdmlJson = "";

                                    if(column.children[i].text === "发送方案") {
                                        rdmlJson = Config.getRdmlByCanId(Config.currentCheckedDeviceCanId)
                                    }

                                    console.log("设备名称：", Config.currentCheckedDeviceName, "，执行动作：", column.children[i].text);
                                    //添加日志
                                    OperateLogManager.addNewOperationRecord(Config.currentCheckedDeviceName, column.children[i].text)
                                    //发送can指令
                                    Config.executeCommand(Config.currentCheckedDeviceCanId, column.children[i].text, rdmlJson)
                                    break;
                                }
                            }
                            comboBox.popup.close()
                        }
                    }

                    Button {
                        id: btnCancel
                        text: "取消"
                        onClicked: {
                            console.log("取消按钮点击")
                            comboBox.popup.close()
                        }
                    }
                }
            }
        }
    }

    ListView {
        id: listView
        width: parent.width
        anchors.top: comboBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        model: Config.deviceList        //注意：Config.deviceList中的元素并不是DeviceButton类，而是device类
        clip: true
        orientation: ListView.Vertical

        delegate: DeviceButton {
            width: parent.width
            deviceName: model.deviceName
            deviceCanId: model.deviceCanId

            Component.onCompleted: {
                if (index === 0) {
                    checked = true;
                }
            }
        }
    }
}
