import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import HcControls
import CustomControls
import TestLogManager

CusTableView {
    id: root

    //日志数据
    property ListModel tableModel: ListModel {}

    model: tableModel

    tableHeaderData: ["名称", "运行时常", "设备名称", "开始时间", "结束时间"]

    tableList: ["SchemeName", "DurationTime", "DeviceName", "StartDateTime", "FinishtDateTime"]

    function getTableModel() {
        tableModel.clear()
        tableModel.append(TestLogManager.enterLogBefore())
    }
}
