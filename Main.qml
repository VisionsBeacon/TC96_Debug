import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import HcControls
import Config
import CustomControls
import DataHandler

Window {
    id: mainWindow
    width: Screen.width - 200
    height: Screen.height - 100
    visible: true


    /*******************功能组件/函数*******************/


    //选择连接模式弹窗
    function showSelectDiag() {
        var dlg = HcMessageDlg.createMessageDlg(mainWindow, qsTr("欢迎使用实验室调试软件，请选择连接类型："))
        dlg.onlyConfirm = false
        dlg.isConnectionDialog = true

        dlg.accepted.connect(function() {
            Config.changeCurrentConnectionType(dlg.canChecked)
        })

        dlg.rejected.connect(function() {
            console.log(qsTr("取消选择连接模式，退出软件"));
            Qt.quit()
        })

        dlg.open()
    }

    //接收读取配置文件结果
    Connections {
        target: DataHandler
        function onParseDevicesConfigCompleted(result) {

            Config.handleParseDevicesConfigResult(result)

            var msg = qsTr("读取配置文件成功！")
            if(!result) {
                msg = qsTr("读取配置文件失败！")
            }

            var dlg = HcMessageDlg.createMessageDlg(mainWindow, msg)
            dlg.onlyConfirm = true
            dlg.open()

            stackLayout.currentIndex = 1
        }
    }


    /********************绘制ui********************/

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        LoginPage {
            id: loginPage
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ActionsPage {
            id: actionPage
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component.onCompleted: {
        stackLayout.currentIndex = 0

        //连接can通信
        if(Config.connecteToCanOpen()) {
            showSelectDiag()
        }
    }
}
