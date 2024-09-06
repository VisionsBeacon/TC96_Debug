import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import HcControls
import CustomControls

Rectangle {
    id: root

    property string deviceName: ""                      // PCR设备名称
    property int deviceCanId: -1                        // PCR设备Id
    color: "transparent"


    /******************************************组件/函数********************************************/

    //获取参数
    function getParams(deviceCanId) {

    }

    //保存参数
    function saveParams(deviceCanId) {

    }


    /******************************************绘制ui********************************************/

    HcButton {
        id: getParamsBtn
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 10
        width: 90
        height: 35
        text: qsTr("获取参数")

        onClicked: {
            console.log("获取", deviceName, "的参数")

            getParams(deviceCanId)
        }
    }

    HcButton {
        id: setParamsBtn
        anchors.top: parent.top
        anchors.left: getParamsBtn.right
        anchors.topMargin: 10
        anchors.leftMargin: 10
        width: 90
        height: 35
        text: qsTr("保存参数")

        onClicked: {
            console.log("保存", deviceName, "的参数")

            saveParams(deviceCanId)
        }
    }

    GroupBox {
        id: sensorBox
        title: qsTr("传感器")
        anchors.top: getParamsBtn.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 10
        width: 650
        height: 100

        RowLayout {
            anchors.fill: parent

            Label {
                id: block1
                width: implicitWidth
                height: 50
                text: qsTr("温度补偿block1：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: block1Text
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Label {
                id: block2
                width: implicitWidth
                height: 50
                text: qsTr("温度补偿block1：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: block2Text
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Label {
                id: block3
                width: implicitWidth
                height: 50
                text: qsTr("温度补偿block1：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: block3Text
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }
        }
    }

    GroupBox {
        id: radiatorBox
        title: qsTr("散热器")
        anchors.top: getParamsBtn.bottom
        anchors.left: sensorBox.right
        anchors.topMargin: 20
        anchors.leftMargin: 20
        width: 200
        height: 100

        Label {
            id: radiatorConpensate
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            width: implicitWidth
            height: 50
            text: qsTr("温度补偿：")
            color: "white"
            font.pixelSize: 13
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            anchors.left: radiatorConpensate.right
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 70
            height: 40
            color: "white"
            border.width: 1
            border.color: "black"

            TextInput {
                id: radiatorConpensateText
                anchors.fill: parent
                width: parent.width
                height: parent.height
                text: ""
                font.pixelSize: 13
                color: "black"
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                leftPadding: 5
            }
        }
    }

    GroupBox {
        id: heatBox
        title: qsTr("加热块")
        anchors.top: getParamsBtn.bottom
        anchors.left: radiatorBox.right
        anchors.topMargin: 20
        anchors.leftMargin: 20
        width: 450
        height: 100

        RowLayout {
            anchors.fill: parent

            Label {
                id: pLabel
                width: implicitWidth
                height: 50
                text: qsTr("P：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: pText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Label {
                id: iLabel
                width: implicitWidth
                height: 50
                text: qsTr("I：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: iText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Label {
                id: dLabel
                width: implicitWidth
                height: 50
                text: qsTr("D：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: dText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }
        }
    }

    GroupBox {
        id: heatLidBox
        title: qsTr("热盖")
        anchors.top: sensorBox.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 10
        width: 400
        height: 200

        RowLayout {
            id: rowLayout1
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2

            Label {
                id: heatLidCompensate
                width: implicitWidth
                height: 50
                text: qsTr("温度补偿：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: heatLidCompensateText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Item {
                width: 20
            }

            Label {
                id: heatLidDistance
                width: implicitWidth
                height: 50
                text: qsTr("距离：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: heatLidDistanceText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            id: rowLayout2
            anchors.top: rowLayout1.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Label {
                id: heatLidP
                width: implicitWidth
                height: 50
                text: qsTr("P：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: heatLidPText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Item {
                width: 20
            }

            Label {
                id: heatLidI
                width: implicitWidth
                height: 50
                text: qsTr("I：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: heatLidIText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Item {
                width: 20
            }

            Label {
                id: heatLidD
                width: implicitWidth
                height: 50
                text: qsTr("D：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: heatLidDText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    GroupBox {
        id: conpensateBox
        title: qsTr("温控补偿")
        anchors.top: sensorBox.bottom
        anchors.left: heatLidBox.right
        anchors.topMargin: 20
        anchors.leftMargin: 20
        width: 300
        height: 100

        RowLayout {
            anchors.fill: parent

            Label {
                id: kLabel
                width: implicitWidth
                height: 50
                text: qsTr("K：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: kText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }

            Label {
                id: bLabel
                width: implicitWidth
                height: 50
                text: qsTr("b：")
                color: "white"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: 70
                height: 40
                color: "white"
                border.width: 1
                border.color: "black"

                TextInput {
                    id: bText
                    width: parent.width
                    height: parent.height
                    text: ""
                    font.pixelSize: 13
                    color: "black"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 5
                }
            }
        }
    }
}
