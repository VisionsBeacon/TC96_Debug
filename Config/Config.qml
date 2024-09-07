pragma Singleton

import QtQuick
import DataHandler 1.0

Item {

    id: root

    /*******************************变量*********************************/

    //连接类型
    enum ConnectionTypeNum {
        None = 0,
        Can,
        Lan
    }

    //指令类型
    enum CommandTypeNum {
        RESET = 0,      //复位
        START,          //方案开始运行
        STOP,           //方案停止
        LID_OPEN,       //开盖
        LID_CLOSE,      //关盖
        HEAT_LID_ON,    //热盖
        HEAT_LID_OFF,   //关热盖
        SET_RDML,       //设置方案
        SET_PARAM,      //设置参数
        READ_PARAM,     //读取参数
        TEMPERATURE     //温度
    }

    //读取温度计时器
    Timer {
        id: temperatureTimer
        interval: 100
        running: false
        repeat: true

        onTriggered: {
            if(currentIndex < currentShowLineDevices.length) {
                getTemperature(currentShowLineDevices[currentIndex])
                currentIndex++
            } else {
                currentIndex = 0
            }
        }
    }

    //抽象出设备组件
    property Component device: Component {
        QtObject {
            property string deviceName: ""
            property int deviceCanId: -1
        }
    }

    //存储自定义的device组件
    property ListModel deviceList: ListModel{}

    //默认连接类型
    property int currentConnectionType: Config.ConnectionTypeNum.None

    //是否成功连接设备
    property bool isConnected: false

    //此时选中的设备名称
    property string currentCheckedDeviceName: ""

    //此时选中的设备can id
    property int currentCheckedDeviceCanId: -1

    //存储一对pcr组件与其canId
    property var pcrMap: {0: 0}

    //存储当前开启曲线的设备名称
    property var currentShowLineDevices: []

    //当前读取温度的序号
    property int currentIndex: 0

    // //存储指令与其索引
    // property var commandMap: ({
    //         "复位":     Config.CommandTypeNum.FUN_RESET,
    //         "开盖":     Config.CommandTypeNum.FUN_LID_OPEN,
    //         "关盖":     Config.CommandTypeNum.FUN_LID_CLOSE,
    //         "发送方案": Config.CommandTypeNum.FUN_SET_RDML,
    //         "开始":     Config.CommandTypeNum.FUN_START,
    //         "停止":     Config.CommandTypeNum.FUN_STOP
    // })


    /*******************************信号*********************************/


    //设备按钮点击信号
    signal sigDeviceButtonClicked(int deviceId);

    //日志返回主页面信号
    signal sigLogReturnToActionsPage();



    /*******************************函数*********************************/

    //发送设备按钮点击信号
    function emitSigDeviceButtonClicked(deviceId) {
        sigDeviceButtonClicked(deviceId)
    }

    //根据主题返回图标的路径
    function iconSource(fileName, addSuffix = true) {
        return `qrc:/qt/qml/TC96_Debug/Icons/${fileName}`
    }

    //接收连接类型返回
    function changeCurrentConnectionType(canChecked) {
        if(canChecked) {
            currentConnectionType = Config.ConnectionTypeNum.Can
            console.log(qsTr("使用Can连接模式"))
        } else {
            currentConnectionType = Config.ConnectionTypeNum.Lan
            console.log(qsTr("使用Lan连接模式"))

            //读取配置文件
            DataHandler.loadingDevicesConfig()

            //启动Lan服务
            DataHandler.startLanServer()
        }
    }

    //处理读取配置文件结果
    function handleParseDevicesConfigResult(result) {
        if(result) {
            //创建PCR设备按钮
            var devicesMap = DataHandler.getDeviceMap()

            for (var key in devicesMap) {
                var device = Config.createDevicesButton({
                    deviceName: key,
                    deviceCanId: devicesMap[key]
                })
                deviceList.append(device)
            }
        }
    }

    //创建组件
    function createObjectWithProperties(component, parent = null, properties = {}) {
        var obj = component.createObject(parent)

        if (obj === null) {
            console.log("Error:", component.errorString())
            return null;
        }

        for (var propName in properties) {
            obj[propName] = properties[propName];
        }

        return obj
    }

    //创建抽象的设备组件
    function createDevicesButton(properties) {
        return createObjectWithProperties(device, null, properties)
    }

    //返回当前组合的RDML
    function getRdmlByCanId(deviceCanId) {
        var pcr = Config.pcrMap[deviceCanId]
        return pcr.toRdml();
    }

    //日志返回主页面
    function emitSigLogReturnToActionsPage() {
        sigLogReturnToActionsPage();
    }

    //添加当前开启曲线的设备，并开启计时器
    function addShowLineDevices(deviceName) {
        currentShowLineDevices.push(deviceName)

        if(currentShowLineDevices.length > 0 && !temperatureTimer.running) {
            temperatureTimer.running = true
        }
    }

    //删除关闭曲线的设备，并关闭计时器
    function removeShowLineDevices(deviceName) {
        for(var device in currentShowLineDevices) {
            if (device === deviceName) {
                currentShowLineDevices.remove(deviceName)

                if(currentShowLineDevices.length === 0) {
                    temperatureTimer.running = false
                }
            }
        }
    }

    //根据指令名称返回其索引值
    // function getCommandType(commandStr) {
    //     var type = Config.commandMap[commandStr]
    //     return type
    // }



    /************************************** can接口 **************************************/


    //下发can指令，参数1："TC01"，参数2："reset"
    function executeCommand(deviceId, commandStr, rdmlJson) {

        var commandType = Config.CommandTypeNum.RESET

        if(commandStr === "复位") {
            commandType = Config.CommandTypeNum.RESET
        } else if(commandStr === "开盖") {
            commandType = Config.CommandTypeNum.LID_OPEN
        } else if(commandStr === "关盖") {
            commandType = Config.CommandTypeNum.LID_CLOSE
        } else if(commandStr === "发送方案") {
            commandType = Config.CommandTypeNum.SET_RDML
        } else if(commandStr === "开始") {
            commandType = Config.CommandTypeNum.START
        } else if(commandStr === "停止") {
            commandType = Config.CommandTypeNum.STOP
        }

        console.log("deviceId: ", deviceId, ", commandType: ", commandType)

        DataHandler.sendCommand(deviceId, commandType, rdmlJson)
    }

    //读取单个PCR温度
    function getTemperature(deviceName) {
        DataHandler.getTemperature(deviceName, "temperature")
    }
}
