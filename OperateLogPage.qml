import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import HcControls
import CustomControls
import OperateLogManager

CusTableView {
    id: root

    //日志数据
    property ListModel tableModel: ListModel {}

    model: tableModel

    tableHeaderData: ["设备名称", "操作详情", "开始时间"]

    tableList: ["DeviceName", "DetailInfo", "StartDateTime"]

    function getTableModel() {
        tableModel.clear()
        tableModel.append(OperateLogManager.enterLogBefore())
    }
}
