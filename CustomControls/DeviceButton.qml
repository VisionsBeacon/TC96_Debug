import QtQuick
import QtQuick.Controls

import Config


Rectangle  {
    id: root

    property string deviceName: ""
    property int deviceCanId: -1
    property bool checked: false            //按钮是否被选中

    implicitHeight: 70
    border.color: "black"
    border.width: 1
    color: checked ? "#5a6367" : "#444d51"  // 根据选中状态修改颜色

    //定义鼠标悬浮动画
    PropertyAnimation {
        id: hoverAnimation
        target: root
        property: "color"
        to: checked ? "#5a6367" : "#606c71"  // 根据选中状态修改目标颜色
        duration: 100
    }

    //定义鼠标按下动画
    PropertyAnimation {
        id: pressAnimation
        target: root
        property: "scale"
        to: 0.95
    }

    //定义鼠标释放动画
    PropertyAnimation {
        id: releaseAnimation
        target: root
        property: "scale"
        to: 1.0
    }



    /***************************绘制ui******************************/

    Label {
        anchors.fill: parent
        font.pixelSize: 20
        text: root.deviceName
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true      //开启鼠标悬浮

        //发送点击信号
        onClicked: {
            checked = !checked
        }

        onPressedChanged: {
            if (pressed) {
                pressAnimation.start();
            } else {
                releaseAnimation.start();
            }
        }

        onEntered: {
            hoverAnimation.start();
        }

        onExited: {
            hoverAnimation.stop();
            root.color = checked ? "#5a6367" : "#444d51";
        }
    }

    //实现同类型按钮互斥
    onCheckedChanged: {
        if (checked) {
            for (var i = 0; i < root.parent.children.length; ++i) {
                var item = root.parent.children[i];
                if (item !== root && item.checked && item instanceof DeviceButton) {
                    item.checked = false;
                }
            }

            //记录此时选中的设备
            Config.currentCheckedDeviceCanId = root.deviceCanId;
            Config.currentCheckedDeviceName = root.deviceName;

            Config.emitSigDeviceButtonClicked(root.deviceCanId);
        }
        root.color = checked ? "#5a6367" : "#444d51";
    }
}



