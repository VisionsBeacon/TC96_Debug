import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import HcControls
import CustomControls

Rectangle {

    id: root
    color: "#1f2424"


    /*************************功能组件/函数*****************************/

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            dateLabel.text = Qt.formatDate(new Date(), "yyyy/M/d")
            timeLabel.text = Qt.formatTime(new Date(), "HH:mm:ss")
        }
    }

    Connections {
        target: Config

        function onSigLogReturnToActionsPage() {
            stackLayout.currentIndex = 0
        }
    }




    /***************************绘制ui*****************************/



    //顶部栏
    Rectangle {
        id: topRec
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: logo.height + 10
        color: "#324a4f"

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10

            //瀚辰logo
            Image {
                id: logo
                Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: implicitHeight
                source: Config.iconSource("logo_hcsci.png")
            }

            Item {
                width: 20
            }

            //分割线
            Rectangle {
                height: parent.height / 3
                implicitWidth: 1
                color: "white"
                opacity: 1
            }

            Item {
                width: 20
            }

            //软件名称
            Label {
                id: softName
                Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: implicitHeight
                y:0
                text: qsTr("实验室运行调试软件")
                color: "white"
                font.bold: true
                font.pixelSize: 30
            }

            Item {
                Layout.fillWidth: true
            }

            //日志按钮
            HcButton {
                id: logBtn
                Layout.preferredWidth: 45
                Layout.preferredHeight: 45
                text: qsTr("日志")

                onClicked: {
                    stackLayout.currentIndex = 1
                    logPage.updateAllLogPage()
                }
            }

            Item {
                width: 20
            }

            // 日期组件
            ColumnLayout {
                spacing: 0

                Label {
                    id: dateLabel
                    text: Qt.formatDate(new Date(), "yyyy/M/d")
                    color: "white"
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Label {
                    id: timeLabel
                    text: Qt.formatTime(new Date(), "HH:mm:ss")
                    color: "white"
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        anchors.top: topRec.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ActPage {
            id: actPage
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        LogPage {
            id: logPage
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component.onCompleted: {
        stackLayout.currentIndex = 0
    }
}
