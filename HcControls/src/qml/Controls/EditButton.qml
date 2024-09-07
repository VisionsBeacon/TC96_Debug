import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.Universal

import HcControls

Button {
    id: control
    property int _radius: 3
    property color fontColor: Constants.fontGreyColor
    property int borderWidth: 1
    property color borderColor: Constants.greyBorder
    property Gradient bgGradient: Constants.buttonUnClickedGradientColor
    property var pcr: null
    property bool editable: false

    background: Rectangle {
        id: _rect

        border.width: borderWidth
        border.color: borderColor
        radius: _radius
        width: control.width
        height: control.height
        gradient: control.enabled ? control.down ? Constants.buttonDownGradientColor :
                                    (control.hovered ? Constants.buttonHoverGradientColor :
                                    (control.checked ? Constants.buttonClickedGradientColor :
                                    (bgGradient ? bgGradient : Constants.buttonUnClickedGradientColor))) : Constants.buttonUnEnableGradientColor
    }

    contentItem: Text {
        id: _text

        text: control.text
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: control.width
        height: control.height
        color: control.hovered ? Constants.fontLightColor : (fontColor ? fontColor : Constants.fontGreyColor)
    }

    onClicked: {
        editable = !editable

        if(editable) {
            console.log("编辑", pcr.deviceName, "的方案")
            pcr.editable = true
            _text.text = "保存方案"
        } else {
            console.log("保存", pcr.deviceName, "的方案")
            pcr.editable = false
            _text.text = "编辑方案"
        }
    }
}
