import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import HcControls
import CustomControls

Rectangle {
    id: root
    color: "#1f2424"

    property var buttonNameList: ["实验日志", "操作日志", "故障日志"]
    property ListModel buttonList: ListModel {}

    //点击按钮后切换页面
    function buttonClick(index) {
        let btn = buttonList.get(index)

        switch (btn.text) {
        case "实验日志":
            console.log("进入实验日志")
            testLogPage.getTableModel()
            break
        case "操作日志":
            console.log("进入操作日志")
            optLogPage.getTableModel()
            break
        case "故障日志":
            console.log("进入故障日志")
            errorLogPage.getTableModel()
            break
        default:
            console.log("未知按钮")
            break
        }

        layout.currentIndex = index
    }

    //更新所有日志页面
    function updateAllLogPage() {
        testLogPage.getTableModel()
        optLogPage.getTableModel()
        errorLogPage.getTableModel()
    }


    /***************************绘制ui******************************/

    //顶部菜单栏
    Item {
        id: item
        height: 40
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.rightMargin: 30
        anchors.leftMargin: 30

        Row {
            spacing: 5
            anchors.left: parent.left

            Repeater {
                model: buttonNameList

                delegate: OperationButton {
                    id: btn
                    text: modelData
                    width: Constants.buttonWidth
                    height: Constants.buttonHeight

                    onSigClicked: {
                        buttonClick(index)
                    }

                    Component.onCompleted: {
                        buttonList.append({"text": modelData, "index": index})
                    }
                }
            }
        }

        Row {
            spacing: 10
            anchors.right: parent.right

            HcButton {
                text: qsTr("返回")
                width: Constants.buttonWidth
                height: Constants.buttonHeight

                onClicked: {
                    Config.emitSigLogReturnToActionsPage()
                }
            }
        }

        Component.onCompleted: {
            buttonClick(0)
        }
    }

    //日志窗口
    Rectangle {
        id: logRec
        anchors.top: item.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 25
        anchors.leftMargin: 30
        anchors.bottomMargin: 25

        color: "#1f2424"

        StackLayout {
            id: layout
            anchors.fill: parent

            //实验日志
            TestLogPage {
                id: testLogPage
            }

            //操作日志
            OperateLogPage {
                id: optLogPage
            }

            //故障日志
            ErrorLogPage {
                id: errorLogPage
            }
        }

        Component.onCompleted: {
            layout.currentIndex = 0
        }
    }
}
