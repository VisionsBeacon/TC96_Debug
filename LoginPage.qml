import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import CustomControls

Rectangle {

    id: root




    /********************绘制ui********************/

    //背景图
    Image {
        anchors.fill: parent
        source: Config.iconSource("backgroud.png")
    }

    //瀚辰logo
    Image {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 10
        width: implicitWidth
        height: implicitHeight
        source: Config.iconSource("logo_hcsci.png")
    }

    //实验室运行调试软件
    Label {
        id: textLabel
        width: implicitWidth
        height: 64
        anchors.centerIn: parent
        text: qsTr("实验室运行调试软件")
        color: "#C1C1C1"
        font.bold: true
        font.pixelSize: 70
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
    }

    //正在加载，请稍后
    Rectangle {
        anchors.top: textLabel.bottom
        anchors.horizontalCenter: textLabel.horizontalCenter
        height: textLabel.height
        width: textLabel.width
        color: "transparent"

        RowLayout {
            anchors.fill: parent

            Item {
                Layout.fillWidth: true
            }

            WaitingAnimationItem {
                id: waitingItem
                visible: Config.currentConnectionType ? false : true
            }

            Label {
                font.pixelSize: 20
                text: qsTr("正在加载，请稍后...")
                color: "#C1C1C1"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
