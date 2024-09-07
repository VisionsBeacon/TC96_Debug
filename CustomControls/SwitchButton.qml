import QtQuick
import QtQuick.Controls

Switch {
    id: curveSwitch

    implicitWidth: 65
    implicitHeight: 30

    checked: false

    property string deviceName: ""
    property int deviceCanId: -1
    property color checkedColor: "#caefef"
    property color unCheckedColor: "#1f5050"

    indicator: Rectangle {
        width: parent.width
        height: parent.height
        radius: height / 2
        color: curveSwitch.checked ? curveSwitch.checkedColor : curveSwitch.unCheckedColor
        border.width: 1
        border.color: curveSwitch.checked ? curveSwitch.checkedColor : curveSwitch.unCheckedColor

        //滑块
        Rectangle {
            x: curveSwitch.checked ? parent.width - width - 2 : 1
            width: curveSwitch.checked ? parent.height - 4 : parent.height - 2
            height: width
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            border.color: "#D5D5D5"

            Behavior on x {
                NumberAnimation { duration: 200 }
            }
        }
    }

    onCheckedChanged: {
        if(checked) {
            console.log(deviceName, "：开启温度曲线")
        } else {
            console.log(deviceName, "：关闭温度曲线")
        }
    }
}
