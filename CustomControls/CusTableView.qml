import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HcControls

Item {
    id: root
    signal myCheckStateChanged(int index, var state)

    signal popupWindow(string message, string text, int index)

    //更改所有选中状态
    signal selectAllCheck

    signal cancelAllCheck

    signal buttonClick(int index)

    signal operationClick(int index, int listIndex)

    signal closeAllItemCheck

    property alias searchBtn: searchBtn
    // property alias answerRow: answerRow

    //model
    property ListModel model: ListModel {}

    property var tableHeaderData: ["名称", "运行时常", "设备名称", "开始时间", "结束时间"]

    property var _buttonList: ["下载"]

    property var _operationList: ["查看"]

    //宽度比值
    property var columnProportion: []

    //list循序表 //model中显示的数据存起来按照list循序显示
    property var tableList

    property int headerHeight: 40

    property int rowsHeight: 40

    property int operationWidth: 300

    property int checkWidth: 100

    property int fontPix: 16

    //临时index
    property int tempIndex: -1

    //删除["delete","deleteSelect",""]
    property string popInfo: ""

    property int _listIndex: 0

    //选项列表
    property var checkList: []


    function isValid() {
        if (model.count + 2 === columnProportion.length) {
            return columnProportion
        } else {
            return []
        }
    }

    //获取想对应的比值大小
    function getColumnProportion(index) {
        let data = isValid()

        if (data.length !== 0) {
            let sum = 0
            for (var i in data) {
                sum += i
            }

            if (sum == 0) {
                return 1 / tableHeaderData.length
            }
            return data[index] / sum
        } else {
            return 1 / tableHeaderData.length
        }
    }



    /***************************绘制ui******************************/

    Rectangle {
        id: _rect
        anchors.fill: parent
        color: Constants.bodyDeepBackground
        border.width: 1
        border.color: Constants.blackBorder
        radius: 5

        //顶部菜单栏
        Item {
            id: _item
            height: 40
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            Row {
                spacing: 10
                anchors.left: _item.left

                Repeater {
                    model: _buttonList
                    HcButton {
                        text: qsTr(_buttonList[index])
                        width: Constants.buttonWidth
                        height: Constants.buttonHeight
                        onClicked: {
                            buttonClick(index)
                        }
                    }
                }
            }

            Row {
                id: row
                spacing: 10
                anchors.right: _item.right

                TextField {
                    id: answerRow
                    height: searchBtn.height
                    selectByMouse: true
                    color: Constants.fontBlackColor
                    focus: true
                    placeholderText: "请输入方案名"
                    verticalAlignment: Text.AlignVCenter
                }

                HcButton {
                    id: searchBtn
                    text: "查找"
                    width: Constants.buttonWidth
                    height: Constants.buttonHeight
                }
            }
        }


        ListView {
            id: listView
            anchors.top: _item.bottom
            anchors.bottom: parent.bottom
            anchors.left: _item.left
            anchors.right: _item.right

            header: headerView

            //流程中需要显示数据的顺序
            boundsBehavior: Flickable.StopAtBounds

            model: root.model.count

            delegate: Rectangle {
                id: delegateRect
                property var _data: root.model.get(index)   //取出一条记录
                property int _index: index
                property bool checked: false

                width: listView.width
                height: rowsHeight
                color: checked ? Constants.tableItemCheckedBgColor : (index % 2 !== 0 ? Constants.tableItemiIntervalBgColor : "transparent")
                border.width: index % 2 !== 0 ? 1 : 0
                border.color: Constants.blackBorder

                Connections {
                    target: root
                    function onCloseAllItemCheck() {
                        delegateRect.checked = false
                    }
                }

                MouseArea {
                    id: area
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        mockClick()
                    }

                    //实现每行互斥
                    function mockClick() {
                        closeAllItemCheck()
                        delegateRect.checked = true
                    }
                }

                Row {
                    Item {
                        id: checkItem
                        width: checkWidth
                        height: rowsHeight

                        CheckBox {
                            id: checkbox
                            anchors.centerIn: parent
                            onCheckStateChanged: {
                                root.myCheckStateChanged(index, checkState)
                            }

                            onClicked: area.mockClick()

                            Connections {
                                target: root

                                function onSelectAllCheck() {
                                    checkbox.checkState = Qt.Checked
                                }

                                function onCancelAllCheck() {
                                    checkbox.checkState = Qt.Unchecked
                                }
                            }
                        }
                    }

                    //一条记录中的关键信息
                    Repeater {
                        id: repeater
                        model: tableHeaderData.length

                        delegate: Item {
                            width: (delegateRect.width - checkWidth - operationWidth) * getColumnProportion(index)
                            height: rowsHeight

                            Text {
                                text: delegateRect._data[root.tableList[index]]     //一条记录中的每个部分
                                font.pixelSize: 12
                                elide: Text.ElideRight
                                leftPadding: 3
                                rightPadding: 3
                                color: Constants.fontLightColor
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    //一行中右侧的操作按钮
                    Item {
                        id: operationItem
                        width: operationWidth
                        height: rowsHeight

                        RowLayout {
                            width: parent.width * 4 / 5
                            height: parent.height * 3 / 5
                            anchors.centerIn: parent

                            Repeater {
                                model: _operationList

                                HcButton {
                                    text: qsTr(_operationList[index])
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    onClicked: {
                                        area.mockClick()
                                        operationClick(index, delegateRect._index)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            onCountChanged: {
                checkList = []
            }
        }

        //表格的header
        Component {
            id: headerView

            Rectangle {
                width: listView.width
                height: headerHeight
                border.width: 1
                border.color: Constants.tableHeadBorderColor
                color: Constants.tableHeadbgColor
                z: 10

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        parent.forceActiveFocus()
                    }
                }

                Row {

                    Item {
                        width: checkWidth
                        height: headerHeight

                        CheckBox {
                            anchors.centerIn: parent

                            onCheckStateChanged: {
                                if (checkState == Qt.Unchecked) {
                                    root.cancelAllCheck()
                                } else {
                                    root.selectAllCheck()
                                }
                            }
                        }
                    }

                    Repeater {
                        id: headerRepeater
                        model: tableHeaderData.length

                        Item {
                            width: (listView.width - checkWidth - operationWidth) * getColumnProportion(index)
                            height: headerHeight

                            Text {
                                width: parent.width - 6
                                height: parent.height
                                font.pixelSize: fontPix
                                text: qsTr(tableHeaderData[index])
                                font.weight: Font.Normal
                                elide: Text.ElideRight
                                color: Constants.fontLightColor
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }
    }
}
