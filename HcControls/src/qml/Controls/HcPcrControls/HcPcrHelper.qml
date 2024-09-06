pragma Singleton

import QtQuick
import QtQuick.Controls

QtObject {
    enum StageType {
        PrePcr = 0,
        Hold,
        Pcr,
        PostPcr,
        Infinite
    }

    property var pcrMap: ({})           //存储HcPcr类

    function setValueByKey(key, value) {
        pcrMap[String(key)] = value
    }

    function getPcrByCanId(key) {
        return pcrMap[String(key)]
    }

    property Component pcrStep: Component {
        QtObject {
            property string name: ""
            property int indexOfPcr: 0
            property int indexOfStage: 0
            property int stageIndexOfPcr: 0
            property int startTemp: 0
            property int endTemp: 0
            property int hours: 0
            property int minutes: 0
            property int seconds: 30
            property double ratio: 3.5
            property bool enableGradient: false
            property double gradientTemp: 0.0
            property double gradientStartTemp: 0.0
            property int gradientStartCycle: 0
            property int gradientCycles: 0
            property bool photographable: false
            property int deviceCanId: -1
            property var pcr: null
        }
    }

    property Component pcrStage: Component {
        QtObject {
            property string name: ""
            property int indexOfPcr: 0
            property int cycles: 0
            property int deviceCanId: -1
            property var pcr: null
            property int type: HcPcrHelper.StageType.Pcr
            property ListModel pcrStepModel: ListModel{}
        }
    }

    signal stageIndexChanged(int index, int offset, bool isInc)
    signal stageTypeChanged(int index, int type)
    signal maxTempChanged(double temp)
    signal runStepChanged(int stageIndex, int stepIndex)

    function emitStageIndexChanged(index, offset, isInc) {
        stageIndexChanged(index, offset, isInc)
    }

    function emitStageTypeChanged(index, type) {
        stageTypeChanged(index, type)
    }

    function emitMaxTempChanged(temp) {
        maxTempChanged(temp)
    }

    function emitRunStepChanged(stageIndex, stepIndex) {
        runStepChanged(stageIndex, stepIndex)
    }

    function deepClone(target) {
        const map = new WeakMap()

        function isObject(target) {
            return (typeof target === 'object' && target ) || typeof target === 'function'
        }

        function clone(data) {
            if (!isObject(data)) {
                return data
            }

//            if ([Date, RegExp].includes(data.constructor)) {
//                return new data.constructor(data)
//            }
            if (typeof data === 'function') {
                return new Function('return ' + data.toString())()
            }

            const exist = map.get(data)
            if (exist) {
                return exist
            }

            if (data instanceof Map) {
                const result = new Map()
                map.set(data, result)
                data.forEach((val, key) => {
                    if (isObject(val)) {
                        result.set(key, clone(val))
                    } else {
                        result.set(key, val)
                    }
                })
                return result
            }

            if (data instanceof Set) {
                const result = new Set()
                map.set(data, result)
                data.forEach(val => {
                    if (isObject(val)) {
                        result.add(clone(val))
                    } else {
                        result.add(val)
                    }
                })
                return result
            }

            const keys = Reflect.ownKeys(data)
            const allDesc = Object.getOwnPropertyDescriptors(data)
            const result = Object.create(Object.getPrototypeOf(data), allDesc)

            map.set(data, result)
            keys.forEach(key => {
                const val = data[key]
                if (isObject(val)) {
                    result[key] = clone(val)
                } else {
                    result[key] = val
                }
            })

            return result
        }

        return clone(target)
    }

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

    function createPcrStepObj(properties) {
        return createObjectWithProperties(pcrStep, null, properties)
    }

    function createPcrStageObj(properties) {
        return createObjectWithProperties(pcrStage, null, properties)
    }
}
