import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

import Config
import HcControls
import CustomControls

Rectangle {
    id: root

    property int pageIndex: -1          //栈中的索引
    property string deviceName: ""
    property int deviceCanId: -1

    color: "transparent"


    /******************************************组件/函数********************************************/

    //带标题的输入框
    component LabelTextEditLine: Row {
        property string label: ""
        property string content: ""

        Label {
            width: parent.width/4
            height: parent.height
            text: parent.label
            color: "white"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            width: parent.width * 3/4
            height: parent.height
            color: "white"
            border.width: 1
            border.color: "black"

            TextInput {
                width: parent.width
                height: parent.height
                text: ""
                font.pixelSize: 14
                color: "black"
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 5

                onEditingFinished: {
                    parent.parent.content = text
                }
            }
        }
    }





    /****************************************绘制ui*****************************************/


    Rectangle {
        id: topRec
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 70
        color: "transparent"

        RowLayout {
            anchors.fill: parent

            Item {
                Layout.fillWidth: true
            }

            Label {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("温度曲线")
                font.pixelSize: 16
                color: "white"
            }

            Item {
                width: 5
            }

            //温度曲线开关
            SwitchButton {
                id: curveSwitch
                Layout.preferredWidth: 65
                Layout.preferredHeight: 30
                deviceCanId: root.deviceCanId
                deviceName: root.deviceName
            }

            Item {
                width: 70
            }

            LabelTextEditLine {
                id: heatLidInput
                Layout.preferredWidth: 100
                Layout.preferredHeight: 30
                label: qsTr("热盖温度: ")
            }

            Item {
                width: 20
            }

            //放大按钮
            HcButton {
                id: bigBtn
                Layout.preferredWidth: 40
                Layout.preferredHeight: 40
                text: qsTr("放大")
            }

            Item {
                width: 5
            }

            //导出
            HcButton {
                id: exportBtn
                Layout.preferredWidth: 40
                Layout.preferredHeight: 40
                text: qsTr("导出")
            }

            Item {
                width: 20
            }
        }
    }

    CurvePage {
        id: curveRec
        anchors.top: topRec.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height / 2 - topRec.height - pcrBtn.height
        deviceName: root.deviceName
        deviceCanId: root.deviceCanId
        showLine: curveSwitch.checked
        color: "transparent"
    }

    OperationButton {
        id: pcrBtn
        text: "温控程序"
        anchors.top: curveRec.bottom
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 15
        width: 100
        height: 45

        onSigClicked: {
            stackLayout.currentIndex = 0
        }
    }

    OperationButton {
        id: paramsBtn
        text: "参数调整"
        anchors.top: curveRec.bottom
        anchors.left: pcrBtn.right
        anchors.topMargin: 10
        anchors.leftMargin: 5
        width: 100
        height: 45

        onSigClicked: {
            stackLayout.currentIndex = 1
        }
    }

    Rectangle {
        id: pcrRec
        anchors.top: pcrBtn.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        radius: 5
        color: "#374246"

        StackLayout {
            id: stackLayout
            anchors.fill: parent

            HcPcr {
                id: pcr
                deviceName: root.deviceName
                deviceCanId: root.deviceCanId
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            ParamsPage {
                id: paramsPage
                deviceName: root.deviceName
                deviceCanId: root.deviceCanId
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Component.onCompleted: {
        pcrBtn.checked = true
        Config.pcrMap[root.deviceCanId] = pcr;
    }
}
