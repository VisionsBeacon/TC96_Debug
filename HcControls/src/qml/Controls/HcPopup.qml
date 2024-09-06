import QtQuick 2.15
import QtQuick.Controls 2.15

Popup {
    property string title: qsTr("prompt")
    property int headerRadius: 10
    property color headerColor: "#a1e9ee"
    property color headerBorderColor: "#b7d4d4"
    property int headerFontSize: 20
    property color headerFontColor: "#484848"
    property Gradient grad: Constants.adminHeadGradientColor

    id: _popup
    width: implicitWidth
    height: implicitHeight
    anchors.centerIn: parent
    padding: 0 - headerRadius
    parent: Overlay.overlay
    closePolicy: Popup.NoAutoClose/* | Popup.CloseOnEscape*/

    // 定义弹窗总的样式，如添加圆角
    Rectangle {
        anchors.fill: parent
        radius: headerRadius
        color: Constants.bodyBackground
        border.width: 1
        border.color: headerBorderColor
//        gradient: grad
    }

    // 仅定义头部矩形框
    Rectangle {
        width: parent.width
        height: 50
        color: headerColor
        border.width: 1
        border.color: headerBorderColor
        radius: headerRadius

        Canvas {
            anchors.fill: parent
            onPaint: {
                // 如果存在圆角，则将下半部的圆角填充为矩形
                if (headerRadius != 0) {
                    var ctx = getContext("2d")

                    ctx.fillStyle = headerColor
                    ctx.clearRect(1, headerRadius + 1, parent.width - 2, parent.height - headerRadius - 1)
                    ctx.fillRect(1, headerRadius + 1, parent.width - 2, parent.height - headerRadius - 1)
                }
            }
        }

        Label {
            width: parent.width - 2
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 1
            text: title
            color: headerFontColor
            font.pixelSize: headerFontSize
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            padding: 6
        }
    }
}
