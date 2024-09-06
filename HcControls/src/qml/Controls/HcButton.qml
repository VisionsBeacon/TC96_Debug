import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.Universal

Button {

    id: control
    property int _radius: 3
    property color fontColor: Constants.fontGreyColor
    property int borderWidth: 1
    property color borderColor: Constants.greyBorder
    property Gradient bgGradient: Constants.buttonUnClickedGradientColor

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
}
