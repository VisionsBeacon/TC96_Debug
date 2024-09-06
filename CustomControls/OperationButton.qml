import QtQuick
import QtQuick.Controls


Rectangle  {
    id: root

    property string text: ""
    property bool checked: false            //按钮是否被选中

    implicitHeight: 45
    border.color: "black"
    border.width: 1
    color: checked ? "#374246" : "#2c3535"  // 根据选中状态修改颜色

    //定义鼠标悬浮动画
    PropertyAnimation {
        id: hoverAnimation
        target: root
        property: "color"
        to: checked ? "#2c3535" : "#374246"  // 根据选中状态修改目标颜色
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


    //对外发出点击信号
    signal sigClicked()



    /***************************绘制ui******************************/

    Label {
        anchors.fill: parent
        font.pixelSize: 13
        text: root.text
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true      //开启鼠标悬浮

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
            root.color = checked ? "#374246" : "#2c3535";
        }
    }

    //互斥只对同一个父组件中的按钮生效
    onCheckedChanged: {
        if (checked) {
            for (var i = 0; i < root.parent.children.length; ++i) {
                var item = root.parent.children[i];
                if (item !== root && item.checked && item instanceof OperationButton) {
                    item.checked = false;
                }
            }
        root.sigClicked();
        }
        root.color = checked ? "#374246" : "#2c3535";
    }
}



