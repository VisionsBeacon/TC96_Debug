pragma Singleton

import QtQuick
import DataHandler 1.0
// import Service 1.0

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
        Reset = 0,
        Open_Gate,
        Close_Gate,
        Send_Rdml,
        Start,
        Stop
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
            // DataHandler.loadingDevicesConfig()

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

    //根据当前组合的RDML
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



    /************************************** can接口 **************************************/

    //连接can通信
    function connecteToCanOpen() {



        return true
    }

    //下发can指令，参数1："TC01"，参数2："reset"
    function executeCommand(deviceName, commandStr, rdmlJson) {

        var commandEng = ""

        if(commandStr === "复位") {
            commandEng = "/device/DevInit"
        } else if(commandStr === "开盖") {
            commandEng = "/device/OpenLid"
        } else if(commandStr === "关盖") {
            commandEng = "/device/CloseLid"
        } else if(commandStr === "发送方案") {
            commandEng = "/device/setRdml"
        } else if(commandStr === "开始") {
            commandEng = "/device/PCRStart"
        } else if(commandStr === "停止") {
            commandEng = "/device/PCRStop"
        }

        DataHandler.sendCommand(deviceName, commandEng, rdmlJson)
    }

    //读取单个PCR温度
    function getTemperature(deviceName) {
        DataHandler.getTemperature(deviceName, "temperature")
    }
}
