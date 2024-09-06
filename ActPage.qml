import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import CustomControls

Rectangle {
    id: root
    color: "transparent"


    /*************************功能组件/函数*****************************/

    Connections {
        target: Config
        function onSigDeviceButtonClicked(deviceCanId) {
            var pageIndex = -1;
            for (var i = 0; i < stackLayout.count; i++) {
                var page = stackLayout.itemAt(i);
                if (page.deviceCanId === deviceCanId) {
                    pageIndex = page.pageIndex;
                    break;
                }
            }

            if (pageIndex !== -1) {
                stackLayout.currentIndex = pageIndex;
            }
        }
    }


    /***************************绘制ui*****************************/

    //左侧设备栏
    DevicesPage {
        id: leftRec
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    //参数窗口
    Rectangle {
        id: paramsRec
        anchors.top: parent.top
        anchors.left: leftRec.right
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        color: "transparent"

        //用栈来存放一对温度曲线和温控程序
        StackLayout {
            id: stackLayout
            anchors.fill: parent

            Repeater {
                model: Config.deviceList

                OperatorPage {
                    deviceCanId: model.deviceCanId
                    deviceName: model.deviceName
                    pageIndex: index
                }
            }

            Component.onCompleted: {
                stackLayout.currentIndex = 0
            }
        }
    }
}
