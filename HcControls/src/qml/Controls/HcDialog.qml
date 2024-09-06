import QtQuick
import QtQuick.Controls

Dialog {
    property string message: ""
    property bool onlyConfirm: false

    property bool isConnectionDialog: false
    property bool canChecked: false
    property bool lanChecked: false

    id: cusDialog
    closePolicy: Popup.NoAutoClose
    modal: true
    anchors.centerIn: parent

    background: Rectangle {
        opacity: 0
    }

    contentItem: Rectangle {
        id: background
        radius: 6
        color: Constants.bodyBackground
        border.width: 1
        border.color: Constants.greyBorder

        // 仅定义头部矩形框
        Rectangle {
            id: _header
            width: parent.width
            height: 50
            color: "#a1e9ee"
            border.width: 1
            border.color: "#b7d4d4"
            radius: parent.radius
            anchors.top: parent.Top

            Canvas {
                anchors.fill: parent
                onPaint: {
                    // 如果存在圆角，则将下半部的圆角填充为矩形
                    if (parent.radius !== 0) {
                        var ctx = getContext("2d")

                        ctx.fillStyle = "#a1e9ee"
                        ctx.clearRect(1, parent.radius + 1, parent.width - 2, parent.height - parent.radius - 1)
                        ctx.fillRect(1, parent.radius + 1, parent.width - 2, parent.height - parent.radius - 1)
                    }
                }
            }

            Label {
                width: parent.width - 2
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 1
                text: qsTr("提示")
                color: "#484848"
                font.pixelSize: 18
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                padding: 6
            }
        }

        Column {
            width: parent.width
            height: parent.height - _header.height
            anchors.top: _header.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: warningText
                width: parent.width - 60
                height: cusDialog.isConnectionDialog ? parent.height / 3 : parent.height * 2 / 3
                text: message
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 16
                color: Constants.fontGreyColor
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height / 3
                visible: cusDialog.isConnectionDialog

                CheckBox {
                    id: canCheckBox
                    text: "CAN"
                    checked: cusDialog.canChecked
                    onCheckedChanged: {
                        cusDialog.canChecked = checked
                        if (checked) {
                            lanCheckBox.checked = false
                        }
                    }
                }

                CheckBox {
                    id: lanCheckBox
                    text: "LAN"
                    checked: cusDialog.lanChecked
                    onCheckedChanged: {
                        cusDialog.lanChecked = checked
                        if (checked) {
                            canCheckBox.checked = false
                        }
                    }
                }
            }

            Row {
                id: row
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 200
                height: parent.height / 3

                HcButton {
                    id: confirmBtn
                    width: 100
                    height: 40
                    text: qsTr("确认")
                    font.pixelSize: 14
                    onClicked: {
                        if(!cusDialog.isConnectionDialog) {
                            cusDialog.accept()
                        } else if(canCheckBox.checked || lanCheckBox.checked) {
                            cusDialog.accept()
                        }
                    }
                }

                HcButton {
                    id: cancelBtn
                    width: 100
                    height: 40
                    visible: onlyConfirm ? false : true
                    text: qsTr("取消")
                    font.pixelSize: 14
                    onClicked: cusDialog.rejected()
                }
            }
        }
    }
}
