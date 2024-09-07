import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Config
import HcControls
import CustomControls
import ErrorLogManager

CusTableView {
    id: root

    //日志数据
    property ListModel tableModel: ListModel {}

    model: tableModel

    tableHeaderData: ["故障信息", "故障等级", "故障码", "时间"]

    tableList: ["FaultInfo", "Level", "FaultCode", "DateTime"]

    function getTableModel() {
        tableModel.clear()
        tableModel.append(ErrorLogManager.enterLogBefore())
    }
}
