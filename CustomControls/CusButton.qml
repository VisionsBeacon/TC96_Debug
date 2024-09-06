import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.Universal

import HcControls

Button {

    id: control
    property int _radius: 3

    background: Rectangle {
        radius: _radius
        width: control.width
        height: control.height
        gradient: control.enabled ? control.down ? Constants.buttonDownGradientColor : (control.hovered ? Constants.buttonHoverGradientColor : (control.checked ? Constants.buttonClickedGradientColor : Constants.buttonUnClickedGradientColor)) : Constants.buttonUnEnableGradientColor
    }

    contentItem: Text {
        text: control.text
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: control.width
        height: control.height
        color: control.hovered ? Constants.fontLightColor : Constants.fontBlackColor
    }
}
