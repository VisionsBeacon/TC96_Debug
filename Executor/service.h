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

#include "canfestival.h"
#include "eds_master.h"

#ifdef __cplusplus
};
#endif


//tc96 控制功能枚举
enum FUNNAME {
    FUN_RESET = 0,        //复位
    FUN_START,          //方案开始运行
    FUN_STOP,           //方案停止
    FUN_LID_OPEN,       //开盖
    FUN_LID_CLOSE,      //关盖
    FUN_HEAT_LID_ON,    //热盖
    FUN_HEAT_LID_OFF,   //热盖关
    FUN_SET_RDML,       //设置方案
    FUN_SET_PARAM,      //设置参数
    FUN_READ_PARAM,     //获取参数
    FUN_TEMPERATURE,    //获取温度
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
    void init();
    void initConnect();
    void CanopenInit();
    void InitSdoClients();
    void RegCallbacks(CO_Data* d);
    //读取温度
    void getTemperature(const QString &deviceName);
    //获取参数
    void getParams(const QString &deviceName);
    static UNS32 callback(CO_Data* d,const indextable *table,UNS8 bSubindex);
    static UNS32 warning_callback(CO_Data *d, const indextable *table, UNS8 bSubindex);
    static void OnHeartbeatError(CO_Data* d,UNS8 heartbeatID);
    static void OnSlaveBootup(CO_Data* d,UNS8 nodeId);
    static void ProcessEvents(int canId,int event);

signals:
    void ReadTemperature();

private slots:
    //开启can服务
    void onSigStartLanServer();
    //接收can指令
    void onSigSendNormalCanCommand(int commandType, const QJsonObject &obj);

private:
    static Service* m_Ctrl;
    void InterfaceComplete(FUNNAME, QJsonObject);
};

#endif // SERVICE_H
