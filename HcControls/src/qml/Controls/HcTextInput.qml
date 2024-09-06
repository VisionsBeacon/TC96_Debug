import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: parent.width
    height: parent.height

    property bool hasLabel: true
    property string label: ""
    property string content: ""
    property int padding: 0
    property int fontSize: 14
    property color fontColor: Constants.fontGreyColor
    property color bgColor: "transparent"
    property int borderWidth: 1
    property color borderColor: Constants.greyBorder
    property int echo: TextInput.Normal
    property string regexp: ".*"    /*"^[A-Za-z0-9@_]+$"*/
    property int maxLen: 40

    function getInputText() {
        return hasLabel ? _loader.children[0].children[1].children[0].text : _loader.children[0].children[0].text
    }

    function setInputText(text) {
        if (hasLabel) {
            _loader.children[0].children[1].children[0].text = text
        } else {
            _loader.children[0].children[0].text = text
        }
    }

    Component {
        id: _hasLabel

        Row {
            width: parent.width
            height: parent.height
            spacing: 20

            Label {
                width: parent.width / 4
                height: parent.height
                text: label
                font.pixelSize: fontSize
                color: fontColor
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Normal
            }

            Rectangle {
                width: parent.width * 3 / 4
                height: parent.height
                border.width: borderWidth
                border.color: borderColor
                color: bgColor

                TextInput {
                    id: _textInput1
                    width: parent.width - 2
                    height: parent.height
                    text: content
                    font.pixelSize: fontSize
                    color: fontColor
                    echoMode: echo
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.weight: Font.Normal
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    padding: padding
                    clip: true

                    //字符限制长度
                    maximumLength: maxLen
                    validator: RegularExpressionValidator { regularExpression: RegExp(regexp) }
                    focus: true
                }
            }
        }
    }

    Component {
        id: _noLabel

        Rectangle {
            width: parent.width
            height: parent.height
            border.width: borderWidth
            border.color: borderColor
            color: bgColor

            TextInput {
                id: _textInput2
                width: parent.width - 2
                height: parent.height
                text: content
                font.pixelSize: fontSize
                color: fontColor
                echoMode: echo
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Normal
                anchors.left: parent.left
                anchors.leftMargin: 2

                //字符限制长度
                maximumLength: 30
                validator: RegularExpressionValidator { regularExpression: RegExp(regexp) }
                focus: true
            }
        }
    }

    Loader {
        id: _loader
        anchors.fill: parent
        sourceComponent: hasLabel ? _hasLabel : _noLabel
    }
}
