import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import HcControls

Rectangle {
    property int stepWidth: 200                         // step宽度
    property int stepHeight: 300                        // step高度
    property int roomTemp: 25                           // 室温，添加第一个pcrStep时设置初始温度
    property int maxTemp: 100                           // 最高温度，用于计算温度线高度
    property double coefficient: 1.0                    // 温度系数，用于计算最高温度
    property bool preRead: false                        // 包含Pre-PCR
    property bool postRead: false                       // 包含Post-PCR
    property double reactVol: 10                        // 反应体积
    property double lidTemp: 105                        // 热盖温度
    property bool editable: false                        // 能否编辑方案
    property var pcrStageArr: []                        // pcrstage数组，添加引用，防止对象被回收
    property ListModel pcrStageModel: ListModel{}       // pcrstage数据
    property string deviceName: ""                      // PCR设备名称
    property int deviceCanId: -1                        // PCR设备Id


    function updateStageCycles(index, cycles) {
        var stage = control.pcrStageModel.get(index)

        if (stage) {
            stage.cycles = cycles
        }
    }

    function updateStageType(index, type) {
        var stage = control.pcrStageModel.get(index)

        if (stage) {
            stage.type = type
        }

        if (stage.type === HcPcrHelper.StageType.PostPcr || stage.type === HcPcrHelper.StageType.Infinite) {
            for (var i = 0; i < stage.pcrStepModel.count; i++) {
                var step = stage.pcrStepModel.get(i)

                if (stage.type === HcPcrHelper.StageType.PostPcr) {
                    step.photographable = true
                } else {
                    step.photographable = false
                }
            }
        }

        HcPcrHelper.emitStageTypeChanged(index, type)
    }

    function updateStageIndex(stageIndex, isInsert) {
        for (var i = stageIndex + 1; i < control.pcrStageModel.count; i++) {
            var stage = control.pcrStageModel.get(i)

            if (isInsert) {
                stage.indexOfPcr++
            } else {
                stage.indexOfPcr--
            }
        }
    }

    function appendStage(stageObj) {
        control.pcrStageModel.append(stageObj)
        control.pcrStageArr.push(stageObj)
    }

    function insertStage(index, stageObj) {
        if (index < 0 || (index !== 0 && index > control.pcrStageModel.count)) {
            return
        }

        control.pcrStageModel.insert(index, stageObj)
        control.pcrStageArr.splice(index, 0, stageObj)

        // 更新stage的索引
        for (var i = index + 1; i < control.pcrStageModel.count; i++) {
            var stage = control.pcrStageModel.get(i)

            stage.indexOfPcr++

            // 更新每一个stage中的Step索引，添加stage时，默认会增加一个step，indexOfPcr会增加，所以这里只需要增加stageIndexOfPcr
            for (var j = 0; j < stage.pcrStepModel.count; j++) {
                var step = stage.pcrStepModel.get(j)

                step.stageIndexOfPcr++
            }
        }
    }

    function removeStage(index) {
        if (index < 0 || (index !== 0 && index + 1 > control.pcrStageModel.count)) {
            return
        }

        var stage = control.pcrStageModel.get(index)
        var stepCnt = stage.pcrStepModel.count

        control.pcrStageModel.remove(index)
        control.pcrStageArr.splice(index, 1)

        // 更新索引
        for (var i = index; i < control.pcrStageModel.count; i++) {
            stage = control.pcrStageModel.get(i)
            stage.indexOfPcr--

            for (var j = 0; j < stage.pcrStepModel.count; j++) {
                var step = stage.pcrStepModel.get(j)

                // 更新Step的索引
                step.stageIndexOfPcr--
                step.indexOfPcr = step.indexOfPcr - stepCnt
            }
        }
    }

    function removeAllStage() {
        while (control.pcrStageModel.count > 0) {
            removeStage(0)
        }
    }

    function findCurStep(stageIndex, stepIndex) {
        var stage = control.pcrStageModel.get(stageIndex)
        var step = null

        if (stage) {
            step = stage.pcrStepModel.get(stepIndex)
        }

        return step
    }

    function findPreStep(stageIndex, stepIndex) {
        var stage = control.pcrStageModel.get(stageIndex)
        var preStep = null
        var isFind = false

        if (stage) {
            if (stage.pcrStepModel.count > 0) {
                if (stepIndex - 1 >= 0) {
                    preStep = stage.pcrStepModel.get(stepIndex - 1)
                    isFind = true
                }
            }
        }

        if (!isFind) {
            if (stageIndex - 1 >= 0) {
                stage = control.pcrStageModel.get(stageIndex - 1)
                preStep = stage.pcrStepModel.get(stage.pcrStepModel.count - 1)
            }
        }

        return preStep
    }

    function findNextStep(stageIndex, stepIndex) {
        var stage = control.pcrStageModel.get(stageIndex)
        var nextStep = null
        var isFind = false

        if (stage) {
            if (stage.pcrStepModel.count > stepIndex + 1) {
                nextStep = stage.pcrStepModel.get(stepIndex + 1)
                isFind = true
            }
        }

        if (!isFind) {
            if (control.pcrStageModel.count > stageIndex + 1) {
                stage = control.pcrStageModel.get(stageIndex + 1)
                nextStep = stage.pcrStepModel.get(0)
            }
        }

        return nextStep
    }

    function updateStepIndex(stageIndex, stepIndex, isInsert) {
        var stepModel = control.pcrStageModel.get(stageIndex).pcrStepModel

        if (stepModel) {
            for (var i = stepIndex + 1; i < stepModel.count; i++) {
                if (i < 0) {
                    continue
                }

                var step = stepModel.get(i)

                if (step) {
                    if (isInsert) {
                        step.indexOfStage++
                        step.indexOfPcr++
                    } else {
                        step.indexOfStage--
                        step.indexOfPcr--
                    }
                }
            }
        }

        for (var j = stageIndex + 1; j < control.pcrStageModel.count; j++) {
            stepModel = control.pcrStageModel.get(j).pcrStepModel
            if (stepModel) {
                for (var k = 0; k < stepModel.count; k++) {
                    step = stepModel.get(k)

                    if (step) {
                        if (isInsert) {
                            step.indexOfPcr++
                        } else {
                            step.indexOfPcr--
                        }
                    }
                }
            }
        }
    }

    // 插入Step时，直接更改温度，界面不重绘，采用先删后增方式来规避
    function updateStepStartTemp(stageIndex, stepIndex, temp) {
        var stage = control.pcrStageModel.get(stageIndex)

        if (stage) {
            var stepModel = stage.pcrStepModel

            if (stepModel && stepModel.count > 0) {
                var delObj = stepModel.get(stepIndex)
                var addObj = HcPcrHelper.createPcrStepObj({
                                name: delObj.name,
                                stageIndexOfPcr: delObj.stageIndexOfPcr,
                                indexOfPcr: delObj.indexOfPcr,
                                indexOfStage: delObj.indexOfStage,
                                startTemp: temp,
                                endTemp: delObj.endTemp,
                                hours: delObj.hours,
                                minutes: delObj.minutes,
                                seconds: delObj.seconds,
                                ratio: delObj.ratio,
                                enableGradient: delObj.enableGradient,
                                gradientTemp: delObj.gradientTemp,
                                gradientStartTemp: delObj.gradientStartTemp,
                                gradientStartCycle: delObj.gradientStartCycle,
                                gradientCycles: delObj.gradientCycles,
                                photographable: delObj.photographable,
                                pcr: control
                            })

                stepModel.remove(stepIndex)
                stepModel.insert(stepIndex, addObj)
            }
        }
    }

    function updateStepEndTemp(stageIndex, stepIndex, temp) {
        var curStep = control.findCurStep(stageIndex, stepIndex)
        var nextStep = control.findNextStep(stageIndex, stepIndex)

        if (curStep) {
            curStep.endTemp = temp

            if (temp > control.maxTemp) {
                control.maxTemp = control.coefficient * temp
                HcPcrHelper.emitMaxTempChanged(control.maxTemp)
            }

            if (nextStep) {
                control.pcrStageModel.get(nextStep.stageIndexOfPcr).pcrStepModel.set(nextStep.indexOfStage, {startTemp: temp})
            }
        }
    }

    function updateStepHours(stageIndex, stepIndex, hours) {
        var step = findCurStep(stageIndex, stepIndex)

        if (step) {
            step.hours = hours
        }
//        control.pcrStepModel.set(index, {hours: hours})
    }

    function updateStepMinutes(stageIndex, stepIndex, minutes) {
        var step = findCurStep(stageIndex, stepIndex)

        if (step) {
            step.minutes = minutes
        }
    }

    function updateStepSeconds(stageIndex, stepIndex, seconds) {
        var step = findCurStep(stageIndex, stepIndex)

        if (step) {
            step.seconds = seconds
        }
    }

    // 该接口仅用于添加第一个Step
    function appendStep(stageIndex, stepObj) {
        insertStep(stageIndex, 0, stepObj)
    }

    // 需求是在指定位置后插入节点，而ListModel的insert方法是在之前插入，所以外部调用接口需要将stepIndex加1
    function insertStep(stageIndex, stepIndex, stepObj) {
        var stage = control.pcrStageModel.get(stageIndex)
        var preStep = findPreStep(stageIndex, stepIndex)

        //指的是未插入时，该位置的下一个节点，所以要-1
        var nextStep = findNextStep(stageIndex, stepIndex - 1)

        if (!stage || (stepIndex !== 0 && stepIndex > stage.pcrStepModel.count)) {
            console.log("stage = ", stage, ", stepIndex = ", stepIndex, ", stage.pcrStepModel.count = ", stage.pcrStepModel.count)
            return
        }

        if (preStep) {
            stepObj.startTemp = preStep.endTemp
        }

        if (nextStep) {
            // 存在后续节点，则更新后续节点起始温度
            updateStepStartTemp(nextStep.stageIndexOfPcr, nextStep.indexOfStage, stepObj.endTemp)
        }

        // 插入Step
        stage.pcrStepModel.insert(stepIndex, stepObj)

        // 更新索引
        updateStepIndex(stageIndex, stepIndex, true)
    }

    function removeStep(stageIndex, stepIndex) {
        var stage = control.pcrStageModel.get(stageIndex)
        var preStep = findPreStep(stageIndex, stepIndex)
        var nextStep = findNextStep(stageIndex, stepIndex)

        if (!stage || stepIndex + 1 > stage.pcrStepModel.count) {
            return
        }

        if (nextStep) {
            var temp = nextStep.startTemp

            // 存在后续节点，则更新后续节点起始温度
            if (preStep) {
                temp = preStep.endTemp
            }
            updateStepStartTemp(nextStep.stageIndexOfPcr, nextStep.indexOfStage, temp)
        }

        // 插入Step
        stage.pcrStepModel.remove(stepIndex)

        // 更新索引
        updateStepIndex(stageIndex, stepIndex - 1, false)
    }

    function getSeconds(hours, minutes, seconds, type) {
        var val = 3600 * hours + 60 * minutes + seconds

        if (type === HcPcrHelper.StageType.Infinite) {
            val = 0xFFFFFFFF     // 假定最大值
        }

        return val
    }

    function getDescription(stageIndex, stepIndex, type) {
        var typeStr = ""

        switch (type) {
            case HcPcrHelper.StageType.PrePcr:
                typeStr = "Pre-Read"
                break
            case HcPcrHelper.StageType.Hold:
                typeStr = "Hold"
                break
            case HcPcrHelper.StageType.Pcr:
                typeStr = "PCR"
                break
            case HcPcrHelper.StageType.PostPcr:
                typeStr = "Post-Read"
                break
            case HcPcrHelper.StageType.Infinite:
                typeStr = "Infinite"
                break
            default:
                console.log("invalid stage type, type =", type)
        }

        return stageIndex + "@" + typeStr + "@" + stepIndex
    }

    function getMessure(photographable, type) {
        var messure = "off"

        if (photographable) {
            // tc384拍照设置，先保留，待业务上统一后确定是否保留
//            switch (type) {
//                case HcPcrHelper.StageType.PrePcr:
//                    messure = "background"
//                    break
//                case HcPcrHelper.StageType.PostPcr:
//                    messure = "endpoint"
//                    break
//                default:
//                    messure = "real time"
//            }
            messure = "real time"
        } else {
            messure = ""
        }

        return messure
    }

    //加载模板
    function loadTemplate(file) {
        // 查找索引号最小的stage
        function findMinIndex(arr) {
            if (arr.length > 0) {
                var minIndex = arr[0].index

                for (var i = 0; i < arr.length; i++) {
                    if (arr.index < minIndex) {
                        minIndex = arr.index
                    }
                }

                for (var j = 0; j < arr.length; j++) {
                    if (arr[j].index === minIndex) {
                        return j
                    }
                }
            }

            return -1
        }

        // step.stageIndex必须与stage.index一致，防止模板文件被错误修改
        function reviseStage(stage) {
            for (var i = 0; i < stage.steps.length; i++) {
                stage.steps[i].stageIndex = stage.index
            }
        }

        function addStep(stageIndex, stepIndex, step) {
            var newStep = HcPcrHelper.createPcrStepObj({
                            indexOfPcr: step.indexOfPcr,
                            indexOfStage: step.indexOfStage,
                            stageIndexOfPcr: step.stageIndex,
                            startTemp: step.startTemp,
                            endTemp: step.endTemp,
                            hours: step.hours,
                            minutes: step.minutes,
                            seconds: step.seconds,
                            ratio: step.ratio,
                            enableGradient: step.enableGradient,
                            gradientTemp: step.gradientTemp,
                            gradientStartTemp: step.gradientStartTemp,
                            gradientStartCycle: step.gradientStartCycle,
                            gradientCycles: step.gradientCycles,
                            photographable: control.pcrStageModel.get(stageIndex).type === HcPcrHelper.StageType.PostPcr ? true : step.photographable,
                            pcr: control
                        })

            insertStep(stageIndex, stepIndex, newStep)
        }

        function addStage(stageIndex, stage) {
            var newStage = HcPcrHelper.createPcrStageObj({
                            indexOfPcr: stage.index,
                            cycles: stage.cycles,
                            deviceCanId: control.deviceCanId,
                            pcr: control,
                            type: stage.type
                        })

            insertStage(stageIndex, newStage)
        }

        var obj = JSON.parse(HcFileOp.readAll(file))

        if (!obj || !obj.pcr) {
            console.log("parse template file failed")
            return
        }

        removeAllStage()

        control.lidTemp = obj.pcr.lidTemperature
        control.reactVol = obj.pcr.reactVolume

        var stages = obj.pcr.stages
        var stageLen = stages.length
        var stageCnt = 0;

        while (stageCnt < stageLen) {
            var minStageIndex = findMinIndex(stages)

            if (minStageIndex < 0) {
                break
            } else {
                var stage = stages[minStageIndex]
                var steps = stage.steps
                var stepLen = steps.length
                var stepCnt = 0

                stage.index = stageCnt
                reviseStage(stage)
                addStage(stageCnt, stage)

                while (stepCnt < stepLen) {
                    var minStepIndex = findMinIndex(steps)

                    if (minStepIndex < 0) {
                        break
                    } else {
                        var step = steps[minStepIndex]

                        step.indexOfStage = stepCnt
                        addStep(stageCnt, stepCnt, step)
                        steps.splice(minStepIndex, 1)
                        stepCnt++
                    }
                }

                stages.splice(minStageIndex, 1)
            }

            stageCnt++
        }
    }

    //保存模板
    function saveTemplate(file) {
        var obj = {pcr: {}}

        obj.pcr["lidTemperature"] = control.lidTemp
        obj.pcr["reactVolume"] = control.reactVol
        obj.pcr["stages"] = []

        for (var i = 0; i < control.pcrStageModel.count; i++) {
            var stage = control.pcrStageModel.get(i)
            var stageObj = {}

            stageObj["index"] = stage.indexOfPcr
            stageObj["cycles"] = stage.cycles
            stageObj["type"] = stage.type
            stageObj["steps"] = []

            for (var j = 0; j < stage.pcrStepModel.count; j++) {
                var step = stage.pcrStepModel.get(j)
                var stepObj = {}

                stepObj["indexOfPcr"] = step.indexOfPcr
                stepObj["indexOfStage"] = step.indexOfStage
                stepObj["stageIndex"] = step.stageIndexOfPcr
                stepObj["startTemp"] = step.startTemp
                stepObj["endTemp"] = step.endTemp
                stepObj["hours"] = step.hours
                stepObj["minutes"] = step.minutes
                stepObj["seconds"] = step.seconds
                stepObj["ratio"] = step.ratio
                stepObj["enableGradient"] = step.enableGradient
                stepObj["gradientTemp"] = step.gradientTemp
                stepObj["gradientStartTemp"] = step.gradientStartTemp
                stepObj["gradientStartCycle"] = step.gradientStartCycle
                stepObj["gradientCycles"] = step.gradientCycles
                stepObj["photographable"] = step.photographable

                stageObj["steps"].push(stepObj)
            }

            obj.pcr["stages"].push(stageObj)
        }

        HcFileOp.writeAll(file, JSON.stringify(obj, null, "\t"))
    }

    //转为RDML结构
    function toRdml() {
        var jsonStr = "{\"thermalCyclingConditions\": {"
        var nr = 1

        jsonStr += "\"id\": \"\"" + ","
        jsonStr += "\"lidTemperature\": " + control.lidTemp + ","
        jsonStr += "\"reactVolume\": " + control.reactVol + ","
        jsonStr += "\"step\": " + "[{"

        for (var i = 0; i < control.pcrStageModel.count; i++) {
            var stage = control.pcrStageModel.get(i)
            var repeat = stage.cycles - 1

            if (stage.pcrStepModel && stage.pcrStepModel.count > 0) {
                for (var j = 0; j < stage.pcrStepModel.count; j++) {
                    var step = stage.pcrStepModel.get(j)

                    jsonStr += "\"nr\": " + nr + ","
                    jsonStr += "\"description\": \"" + getDescription(i, j, stage.type) + "\","
                    jsonStr += "\"temperature\": " + "{"
                    jsonStr += "\"temperature\": " + step.endTemp + ","
                    jsonStr += "\"duration\": " + getSeconds(step.hours, step.minutes, step.seconds, stage.type) + ","
                    jsonStr += "\"temperatureChange\": " + 0 + ","
                    jsonStr += "\"durationChange\": " + 0 + ","
                    jsonStr += "\"measure\": \"" + getMessure(step.photographable, stage.type) + "\","
                    jsonStr += "\"ramp\": " + step.ratio + "}"
                    jsonStr += "},{"

                    nr++
                }

                if (repeat >= 0) {
                    jsonStr += "\"nr\": " + nr + ","
                    jsonStr += "\"loop\": " + "{"
                    jsonStr += "\"goto\": " + (nr - stage.pcrStepModel.count) + ","
                    jsonStr += "\"repeat\": " + repeat + "}"

                    nr++
                } else {
                    console.log("invalid repeat, stageIndex =", i, ", cycles =", stage.cycles)
                }
            }

            if (i < control.pcrStageModel.count - 1) {
                if (control.pcrStageModel.get(i + 1).pcrStepModel.count > 0) {
                    jsonStr += "},{"
                } else {
                    continue
                }

            } else {
                jsonStr += "}]}"
            }
        }

        jsonStr += "}"

        return jsonStr
    }

    id: control
    implicitWidth: parent.width
    implicitHeight: parent.height
    color: "#374246"

    Row {
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 30
        spacing: 20

        CheckBox {
            width: 90
            height: 30
            checkState: Qt.Unchecked
            text: "Pre-Read"
            font.pixelSize: 14
            palette.text: "#5E5E5E"
            palette.windowText: "#DFDFDF"
            indicator.width: 20
            indicator.height: 20

            onClicked: {
                control.preRead = !control.preRead
            }
        }

        CheckBox {
            width: 90
            height: 30
            checkState: Qt.Unchecked
            text: "Post-Read"
            font.pixelSize: 14
            palette.text: "#5E5E5E"
            palette.windowText: "#DFDFDF"

            onClicked: {
                control.postRead = !control.postRead
            }
        }

        EditButton {
            id: editBtn
            text: "编辑方案"
            pcr: control
            width: 90
            height: 35
        }
    }

    ListView {
        id: listView
        width: parent.width
        height: parent.height - 50
        model: control.pcrStageModel
        clip: true
        orientation: ListView.Horizontal        //水平排列
        interactive: false                      //禁止用户交互，使列表不可滚动
        cacheBuffer: 10
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left

        delegate: HcPcrStage {
            indexOfPcr: model.indexOfPcr
            cycles: model.cycles
            type: model.type
            pcr: model.pcr
        }

        ScrollBar.horizontal: ScrollBar {       //添加一个水平滚动条
            policy: listView.contentWidth > listView.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
        }
    }

    onPreReadChanged: {
        if (control.preRead) {
            var stage = HcPcrHelper.createPcrStageObj({
                            indexOfPcr: 0,
                            cycles: 1,
                            deviceCanId: control.deviceCanId,
                            pcr: control,
                            type: HcPcrHelper.StageType.PrePcr
                        })

            control.insertStage(0, stage)
        } else {
            if (control.pcrStageModel.get(0).type !== HcPcrHelper.StageType.PrePcr) {
                console.log("The first stage's type is not pre-pcr, it can not be removed. type =", control.pcrStageModel.get(0).type)
            } else {
                control.removeStage(0)
            }
        }
    }

    onPostReadChanged: {
        var cnt = control.pcrStageModel.count

        if (control.postRead) {
            var postStage = HcPcrHelper.createPcrStageObj({
                            indexOfPcr: cnt,
                            cycles: 1,
                            deviceCanId: control.deviceCanId,
                            pcr: control,
                            type: HcPcrHelper.StageType.PostPcr
                        })
            var infiniteStage = HcPcrHelper.createPcrStageObj({
                            indexOfPcr: cnt + 1,
                            cycles: 1,
                            deviceCanId: control.deviceCanId,
                            pcr: control,
                            type: HcPcrHelper.StageType.Infinite
                        })

            control.appendStage(postStage)
            control.appendStage(infiniteStage)
        } else {
            cnt = control.pcrStageModel.count
            infiniteStage = control.pcrStageModel.get(cnt - 1)
            postStage = control.pcrStageModel.get(cnt - 2)

            if (postStage.type !== HcPcrHelper.StageType.PostPcr ||
                infiniteStage.type !== HcPcrHelper.StageType.Infinite) {
                console.log("The last stage's type is not post-pcr, it can not be removed. type1 =",
                            postStage.type, ", type2 =", infiniteStage.type)
            } else {
                control.removeStage(cnt - 1)
                control.removeStage(cnt - 2)
            }
        }
    }

    Component.onCompleted: {

        //在其他控件中可以通过 HcPcrHelper.getPcrByCanId 访问到该 HcPcr
        HcPcrHelper.setValueByKey(control.deviceCanId, control)

        if (control.pcrStageModel.count === 0) {
            var stage = HcPcrHelper.createPcrStageObj({
                            indexOfPcr: 0,
                            cycles: 1,
                            deviceCanId: control.deviceCanId,
                            pcr: control,
                            type: HcPcrHelper.StageType.Pcr
                        })

            control.appendStage(stage)
        }
    }
}
