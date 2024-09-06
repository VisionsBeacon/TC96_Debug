pragma Singleton

import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: control
    width: 480
    height: 320

    function createMessageDlg(parent, info = "") {
        var obj = _dlgComponent.createObject(parent, {message: info}, )
        return obj
    }

    Component {
        id: _dlgComponent

        HcDialog {
            id: _messageDlg
            width: control.width
            height: control.height
        }
    }
}
