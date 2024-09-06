#ifndef DATADEF_H
#define DATADEF_H

#include <QColor>

#define BASE_NUMBER 8
//设备需要扩大100倍
#define INT_BLOW_UP 100
#define DOUBLE_BLOW_UP 100.00

#define CHANGE_BORDER(widget) {\
    style()->unpolish(widget);\
    style()->polish(widget);\
    widget->update();\
    }

//颜色定义
const QColor UNSTATE_COLOR = QColor(124,124,124);
const QColor EMPTY_COLOR = QColor(62,62,62);
const QColor SUCKER_COLOR = QColor(195,195,195);
const QColor FIRST_COLOR = QColor("#00BBBB");
const QColor SECOND_COLOR = QColor(31,244,19);


enum Position {
    Pos_Clean_Station = 0,
    Pos_Reagent_Sample,
    Pos_Sealer_Film

};

//状态定义
namespace CircleStatus {
//腔室状态
enum CircleState{
    Circle_unState = 0x0000,
    Circle_Normal,
    Circle_Free,
    Circle_Running,
    Circle_Watting,
    Circle_Finish,
    Circle_Empty,
    Circle_Dark,
    Circle_Blue
};
//图例
enum LengendState{
    Lengend_Empty = 0x0010,
    Lengend_Heating,//加热
    Lengend_HeatingOscillation,//加热震荡
    Lengend_Oscillation,//震荡
    Lengend_MagneticAbsorption,//吸磁
    Lengend_Full,
    Lengend_Finish
};
//节点状态
enum NodeState{
    Node_Default = 0x00A0,
    Node_Active,
    Node_Pause,
    Node_Running,
    Node_Error,
    Node_Terminatino,
    Node_Finish
};

//流程状态
enum ProcessState{
    Process_Disable = 0x0100,
    Process_Active,
    Process_Pause,
    Process_Error,
    Process_Terminatino,
    Process_Finish
};

}


enum DP24Number{
    DP24A = 0,
    DP24B = 1
};

struct DP24{
    const int row = 8;//现在默认从坐标0，0开始依次放满，因此使用行列数即可
    const int col = 3;
    int leftPos = 0;
    int topPos  = 0;
    bool size[8][3] = {{false}};
};

struct PosInfo{
    int cavity; //腔室
    int posname; //台面名称
    int row; //行
    int col; //列
    int description; //动作描述 开始和完成
    CircleStatus::CircleState state;//节点动作状态
};

////数据结构定义
//struct CanParam{
//    int index;
//    int subindex;
//    int status;
//    int can_id;
//};

namespace Alarm{
    enum AlarmLevel{
        Alarm_Undefined = 0,
        Alarm_Warning,
        Alarm_Fatal,
        Alarm_UnKnown
    };

    enum AlarmHandleOption{
        Handle_None = 0,
        Handle_Skip = 0x01,
        Handle_Retry = 0x02,
        Handle_Stop = 0x04
    };
}


struct AlarmMessage{
    Alarm::AlarmLevel level;
    QString errorMessage;
    QString handleSuggest;
    QString detailMessage;  //选填
    int option;             //选填
};



#endif // DATADEF_H
