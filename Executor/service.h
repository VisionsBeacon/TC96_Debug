#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>


#include "datadef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../third_party/include/win32/canfestival.h"

#ifdef __cplusplus
};
#endif


//tc96 控制功能枚举
enum FUNNAME {
    FUN_RESET=0,        //复位
    FUN_START,          //方案开始运行
    FUN_STOP,           //方案停止
    FUN_LID_OPEN,       //开盖
    FUN_LID_CLOSE,      //关盖
    FUN_HEAT_LID_ON,    //热盖
    FUN_HEAT_LID_OFF,   //热盖关
    FUN_SET_RDML,       //设置方案

    FUN_SET_PARAM,
    FUN_READ_PARAM,
    FUN_TEMPERATURE,


//    FUN_SET_BLOCKPID,//加热片PID
//    FUN_SET_HEATLIDPID,
//    FUN_SET_BASICPARAMS,
//    FUN_SET_CANPARAMS,
//    FUN_SET_CALIBRATE,
//    FUN_SET_COMPENSATION,//控制参数

//    FUN_READ_BLOCKPID,
//    FUN_READ_HEATLIDPID,
//    FUN_READ_BASICPARAMS,
//    FUN_READ_CANPARAMS,
//    FUN_READ_CALIBRATE,
//    FUN_READ_COMPENSATION,
    
//    FUN_READ_TEMPERATURE1,
//    FUN_READ_TEMPERATURE2,
//    FUN_READ_TEMPERATURE3,
//    FUN_READ_TEMPERATURE4,
//    FUN_READ_TEMPERATURE5,
};

class Service : public QObject
{
    Q_OBJECT
public:
    //需要同步开启的服务
    struct struct_s_BOARD board {
        .busname=(char*)"0",
        .baudrate=(char*)"1M",
    };

    struct reg_s {
        int index;
        int sub_index;
    };

    static const QVector<reg_s> regs;
    static const QVector<reg_s> warning_regs;


public:
    explicit Service(QObject *parent = nullptr);
    static Service* instance();
    void initConnect();
    void CanopenInit();
    void InitSdoClients();
    void RegCallbacks(CO_Data* d);
    static UNS32 callback(CO_Data* d,const indextable *table,UNS8 bSubindex);
    static UNS32 warning_callback(CO_Data *d, const indextable *table, UNS8 bSubindex);
    static void OnHeartbeatError(CO_Data* d,UNS8 heartbeatID);
    static void OnSlaveBootup(CO_Data* d,UNS8 nodeId);
    static void ProcessEvents(int canId,int event);

signals:
    void AsyncComplete(FUNNAME, QJsonObject);
    void ReadTemperature();

private slots:
    //异步调用
    void onAsyncComplete(FUNNAME, QJsonObject);

private:
    static Service* m_Ctrl;
    void InterfaceComplete(FUNNAME, QJsonObject);
};

#endif // SERVICE_H
