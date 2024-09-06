import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts

import HcControls
import CustomControls
import Config

Rectangle {
    id: root
    property string deviceName: ""              // PCR设备名称
    property int deviceCanId: -1                // PCR设备Id
    property var block1Line: null               // block1温度曲线
    property var block2Line: null               // block2温度曲线
    property var block3Line: null               // block3温度曲线
    property var heatLidLine: null              // 热盖温度曲线
    property bool showLine: false               // 是否展示曲线

    property int x_Axis: 0                      // X坐标
    property real initialXMin: 0
    property real initialXMax: 200
    property real initialYMin: 0
    property real initialYMax: 120

    property bool toolTipVisible: false
    property string toolTipText: ""

    function clearAllLines() {
        root.block1Line.clear()
        root.block2Line.clear()
        root.block3Line.clear()
        root.heatLidLine.clear()

        xAxis.max = initialXMax
        x_Axis = initialXMin
    }

    // 为每条曲线添加悬浮显示功能
    function setupHoverToolTip(lineSeries) {
        lineSeries.onHovered.connect(function(point, state) {
            if (state) {
                root.toolTipVisible = true
                root.toolTipText = "X: " + point.x.toFixed(2) + "\nY: " + point.y.toFixed(2)
                toolTip.x = chartView.mapToPosition(point, lineSeries).x + 10
                toolTip.y = chartView.mapToPosition(point, lineSeries).y - 30
            } else {
                root.toolTipVisible = false
            }
        })
    }

    //处理鼠标缩放
    function handleWheel(event) {
        // 缩放因子
        var xScaleFactor = 0.1;

        var delta = event.angleDelta.y;

        // 获取鼠标在图表中的位置
        var mousePos = Qt.point(event.x, event.y);

        // 将鼠标位置映射到数据坐标系
        var mappedMousePos = chartView.mapToValue(mousePos, block1Line);

        // 获取鼠标位置对应的数据点
        var mouseX = mappedMousePos.x;

        // 计算X轴的缩放
        var xRange = xAxis.max - xAxis.min;

        var newXMin = 0;
        var newXMax = 0;

        if (delta > 0) {
            // 放大
            newXMin = mouseX - (mouseX - xAxis.min) * (1 - xScaleFactor);
            newXMax = mouseX + (xAxis.max - mouseX) * (1 - xScaleFactor);

            xAxis.min = Math.max(0, Math.floor(newXMin));
            xAxis.max = Math.max(xAxis.min + 1, Math.floor(newXMax));

        } else if (delta < 0) {
            // 缩小
            newXMin = mouseX - (mouseX - xAxis.min) * (1 + xScaleFactor);
            newXMax = mouseX + (xAxis.max - mouseX) * (1 + xScaleFactor);

            xAxis.min = Math.max(initialXMin, Math.floor(newXMin));
            xAxis.max = Math.min(initialXMax, Math.floor(newXMax));
        }
    }

    Connections {
        target: HttpClient

        function onSandTemperature(device, temperature) {
            if(!showLine || device !== deviceName) {
                return
            } else {
                x_Axis += 2
                if(x_Axis > xAxis.max) {
                    xAxis.max = x_Axis
                }

                root.block1Line.append(x_Axis, temperature);
            }
        }
    }

    onShowLineChanged: {
        if(!showLine) {
            //关闭曲线
            Config.removeShowLineDevices(deviceName)
            clearAllLines()
        } else {
            //开启曲线
            Config.addShowLineDevices(deviceName)
        }
    }


    /*********************************ui绘制**********************************/

    ChartView {
        id: chartView
        anchors.fill: parent
        antialiasing: true

        ValueAxis {
            id: xAxis
            min: root.initialXMin
            max: root.initialXMax
        }

        ValueAxis {
            id: yAxis
            min: root.initialYMin
            max: root.initialYMax
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onWheel: (event) => handleWheel(event)
        }

        // 悬浮显示坐标的文字
        Rectangle {
            id: toolTip
            visible: root.toolTipVisible
            color: "black"
            radius: 5
            border.color: "black"
            z: 10
            width: implicitWidth
            height: implicitHeight

            Text {
                color: "black"
                text: root.toolTipText
                font.pixelSize: 14
            }
        }
    }

    Component.onCompleted: {
        root.block1Line = chartView.createSeries(ChartView.SeriesTypeSpline, "block1", xAxis, yAxis)
        root.block2Line = chartView.createSeries(ChartView.SeriesTypeSpline, "block2", xAxis, yAxis)
        root.block3Line = chartView.createSeries(ChartView.SeriesTypeSpline, "block3", xAxis, yAxis)
        root.heatLidLine = chartView.createSeries(ChartView.SeriesTypeSpline, "heatLid", xAxis, yAxis)

        root.block1Line.color = "red"
        root.block2Line.color = "blue"
        root.block3Line.color = "yellow"
        root.heatLidLine.color = "green"

        // 为每条曲线添加悬浮显示功能
        setupHoverToolTip(root.block1Line)
        setupHoverToolTip(root.block2Line)
        setupHoverToolTip(root.block3Line)
        setupHoverToolTip(root.heatLidLine)
    }
}
