pragma Singleton

import QtQuick 2.15
import QtQuick.Controls 2.15

QtObject {
    //使用桌面的窗口大小，只加载一次
    readonly property int width: Screen.width
    readonly property int height: Screen.height

    readonly property int buttonWidth: 80
    readonly property int buttonHeight: 35

    //用于配置全局颜色

    //全局主体色彩
    readonly property color globalBackground: "#f3f7fb"

    //组件主体背景色
    readonly property color bodyBackground: "#ffffff"

    //组件主体背景深色
    readonly property color bodyDeepBackground: "#353535"

    //组件黑色边框
    readonly property color blackBorder: "#090909"

    //组件白色边框
    readonly property color whiteBorder: "#a0b9b9"

    //节点按钮颜色
    readonly property color unclickedButtonColor: "#ffffff"

    //节点选中边缘样式
    readonly property color clickedButtonBorderColor: "#a0b9b9"

    //admin标签颜色
    readonly property Gradient adminHeadGradientColor: Gradient {
        orientation: Gradient.Horizontal

        GradientStop {
            position: 0
            color: "#00a2ac"
        }

        GradientStop {
            position: 0.48
            color: "#00b3bd"
        }

        GradientStop {
            position: 1
            color: "#009ea8"
        }
    }

    //组件标题渐变
    readonly property Gradient hearGradientColor: Gradient {
        orientation: Gradient.Horizontal
        GradientStop {
            position: 0
            color: "#177b89"
        }
        GradientStop {
            position: 1
            color: "#444545"
        }
    }

    //按钮悬停垂直渐变
    readonly property Gradient buttonHoverGradientColor: Gradient {
        GradientStop {
            position: 0
            color: "#aae2e8"
        }

        GradientStop {
            position: 1
            color: "#aae2e8"
        }
        orientation: Gradient.Vertical
    }

    //选择点击时候
    readonly property Gradient buttonClickedGradientColor: Gradient {
        GradientStop {
            position: 0
            color: "#61ced5"
        }

        GradientStop {
            position: 1
            color: "#61ced5"
        }
        orientation: Gradient.Vertical
    }

    //按下的时候
    readonly property Gradient buttonDownGradientColor: Gradient {
        GradientStop {
            position: 0
            color: "#61ced5"
        }

        GradientStop {
            position: 1
            color: "#61ced5"
        }
        orientation: Gradient.Vertical
    }

    //没有点击时候
    readonly property Gradient buttonUnClickedGradientColor: Gradient {
        GradientStop {
            position: 0
            color: "#f9fbfb"
        }

        GradientStop {
            position: 1
            color: "#d8e2e3"
        }
        orientation: Gradient.Vertical
    }

    //禁止时候
    readonly property Gradient buttonUnEnableGradientColor: Gradient {
        GradientStop {
            position: 0
            color: "#dcdcdc"
        }

        GradientStop {
            position: 1
            color: "#acacac"
        }
        orientation: Gradient.Vertical
    }

    //组件灰色边框
    readonly property color greyBorder: "#636363"

    //字体白色
    readonly property color fontGreyColor: "#484848"

    //字体颜色（浅色）
    readonly property color fontLightColor: "#bdbec0"

    //字体黑色
    readonly property color fontBlackColor: "#000000"

    //字体白色
    readonly property color fontWhiteColor: "#ffffff"

    //选中后字体颜色
    readonly property color clickedFontLightColor: "#008a8a"

    //stack字体选中颜色
    readonly property color stackClickedFontColor: "#ffffff"

    //stack悬停颜色
    readonly property color stackHoverdColor: "#aae2e8"

    //stack选中后颜色
    readonly property color stackClickedColor: "#61ced5"

    //表格中头的边缘
    readonly property color tableHeadBorderColor: "#455f5f"

    //表格中头背景色
    readonly property color tableHeadbgColor: "#354444"

    //表格中间隔背景色
    readonly property color tableItemiIntervalBgColor: "#424242"

    //表格点击背景色
    readonly property color tableItemCheckedBgColor: "#4e6e6e"
}
