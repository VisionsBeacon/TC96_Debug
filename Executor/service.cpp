#include "service.h"
#include "acturator.h"
#include "waiting_list.h"
#include "executorfactory.h"
#include "../eds/eds_master.h"
#include "../Config/datahandler.h"

#include <QThread>

// #include <QtConcurrent/QtConcurrent>

// #include "Frame/messagedialog.h"
// #include "Frame/signalmanager.h"




Service* Service::m_Ctrl = nullptr;

//存储每个索引的状态
const QVector<Service::reg_s> Service::regs = {
    {0x2601, 0x02}, {0x2602, 0x02}, {0x2603, 0x02},
    {0x2604, 0x02}, {0x2605, 0x02}, {0x2606, 0x02},
    {0x2607, 0x02}, {0x2608, 0x02}, {0x2609, 0x02},
    {0x260A, 0x02}, {0x260B, 0x02}, {0x260C, 0x02},
    {0x260D, 0x02}, {0x260E, 0x02}
};

//添加对warning的监视
const QVector<Service::reg_s> Service::warning_regs = {
    {0x2600, 0x05}, {0x2600, 0x03}
};



Service::Service(QObject *parent): QObject(parent)
{
    initConnect();
}

Service *Service::instance()
{
    if(!m_Ctrl)
    {
        m_Ctrl = new Service();
    }

    return m_Ctrl;
}

void Service::initConnect()
{
    connect(this, &Service::AsyncComplete, this, &Service::onAsyncComplete);
}

//初始化
void Service::CanopenInit()
{
    TimerInit();
    InitSdoClients();

    //打开can设备
    board.busname = (char *)DataHandler::getInstance()->zlg_can_ip().toStdString().c_str();

    if(canOpen(&board, &eds_master_Data) == 0)
    {
        //打开can设备失败
        qDebug() << "打开can设备失败";
        exit(0);
    }

    //启动定时器
    eds_master_Data.heartbeatError = OnHeartbeatError;
    eds_master_Data.post_SlaveBootup = OnSlaveBootup;
    RegCallbacks(&eds_master_Data);
    StartTimerLoop(NULL);
    QThread::sleep(1);

    setNodeId(&eds_master_Data, 0x01);
    setState(&eds_master_Data, Initialisation);
}

void Service::InitSdoClients()
{
    UNS32 errcode;
    ODCallback_t *callbacks = NULL;

    int index = 0;
    QVariantMap deviceMap = DataHandler::getInstance()->getDeviceMap();

    for(auto it = deviceMap.begin(); it != deviceMap.end(); ++it)
    {
        QString key = it.key();
        int value = it.value().toInt();
        qDebug() << "Key:" << key << ", Value:" << value;

        // 初始化SDO客户端
        const indextable *entry = scanIndexOD(&eds_master_Data, index + 0x1280, &errcode, &callbacks);

        subindex *subentry = entry->pSubindex;
        *(UNS32 *)subentry[1].pObject = 0x600 + value;
        *(UNS32 *)subentry[2].pObject = 0x580 + value;
        *(UNS8 *)subentry[3].pObject = value;

        entry = scanIndexOD(&eds_master_Data, 0x1016, &errcode, &callbacks);
        subentry = entry->pSubindex;
        *(UNS32 *)subentry[index + 1].pObject = value << 16 | 20000;
        index++;
    }
}

void Service::InterfaceComplete(FUNNAME funName, QJsonObject param)
{
    //统一取index和param {"can_id","param"}
    //提示信息
    //调试一下
   // qDebug()<<"发了一个指令，到这里来了"<<endl;

   //Q_EMIT SignalManager::instance()->AddMessage("发了一个指令，到这里来了");
   //return;

    QString can_id = param["can_id"].toString();

    //先根据id转换为name
    QString name = DataHandler::getInstance()->getDeviceNameById(can_id.toInt());

    switch(funName)
    {
    case FUN_RESET:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "reset");

        //运行
        std::string result = "成功";
        if (e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                result = "失败";
            }
        }
        else
        {
            result = "设备不在线";
        }
        QString func="初始化 ";
        // SignalManager::instance()->AddMessage(func+result.c_str());
    }
        break;
    case FUN_START:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "start");

        // 运行
        std::string result = "成功";
        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                result = "失败";
            }
        }
        else
        {
            result = "设备不在线";
        }

        QString func="方案运行 ";
        // SignalManager::instance()->AddMessage(func+result.c_str());
    }
        break;
    case FUN_STOP:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "stop");

        //运行
        std::string result = "成功";
        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                result = "失败";
            }
        }
        else
        {
            result = "设备不在线";
        }
        QString func="方案停止 ";
        // SignalManager::instance()->AddMessage(func+result.c_str());
    }
        break;
    case FUN_LID_OPEN:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "lid_open");

        //下发指令
        std::string result = "成功";
        if (e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                result = "失败";
            }
        }
        else
        {
            result = "设备不在线";
        }

        QString func = "开盖 ";
        // SignalManager::instance()->AddMessage(func + result.c_str());
    }
        break;
    case FUN_LID_CLOSE:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "lid_close");

        // 运行
        std::string result = "成功";
        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                result = "失败";
            }

        }
        else
        {
            result = "设备不在线";
        }
        QString func="关盖 ";
        // SignalManager::instance()->AddMessage(func+result.c_str());

    }
        break;
    case FUN_SET_PARAM:
    {
        auto e1 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "heatlidpid");
        auto e2 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "blockpid");
        auto e3 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "basicparam");
        auto e4 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "canparam");
        auto e5 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "calibratecompensation");
        auto e6 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "cyclingcompensation");

        //取出需要写的值
        QJsonObject value=param["param"].toObject();
        int32_t block_kp=value["block_kp"].toInt();
        int32_t block_kd=value["block_kd"].toInt();
        int32_t block_ki=value["block_ki"].toInt();
        double lid_move_distance=value["lid_distance"].toString().toDouble();

        //int32_t can_id=value["can_id"].toInt();
        int32_t baundrate=value["baundrate"].toInt();
        int32_t block1=value["block1"].toInt();
        int32_t block2=value["block2"].toInt();
        int32_t block3=value["block3"].toInt();
        int32_t heatLid=value["heatLid"].toInt();
        int32_t radiator=value["radiator"].toInt();
        int32_t k=value["k"].toInt();
        int32_t b=value["b"].toInt();
        int32_t heatlid_kp=value["heatlid_kp"].toInt();
        int32_t heatlid_ki=value["heatlid_ki"].toInt();
        int32_t heatlid_kd=value["heatlid_kd"].toInt();
        // 运行
        std::string result1,result2,result3,result4,result5,result6 = "OK";

        if(e3 != nullptr)
        {
            if(e3->write_data(0,lid_move_distance*1000) != 0)
            {
                result3 = "FAILED";
            }
            else
            {
                // QThread::msleep(200);  //延迟; //加一个延时，避免信号量阻塞未取到值
            }

        }
        else
        {
            result3 = "NO_DEVICE";
        }
    }
        break;
    case FUN_SET_RDML:
    {
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "set_schema");

        // 运行
        std::string result = "成功K";
        if (e != nullptr)
        {

            if(e->run(QString(QJsonDocument(param["param"].toObject()).toJson(QJsonDocument::Compact))) != 0)
            {
                result = "失败";
            }

        }
        else
        {
            result = "设备不在线";
        }
        QString func="方案设置 ";
        // SignalManager::instance()->AddMessage(func+result.c_str());
    }
        break;
    }
}

void Service::onAsyncComplete(FUNNAME name, QJsonObject var)
{
    //qDebug() << "接口调用:" << name << "参数：" << var;
    // QtConcurrent::run(this, &Service::InterfaceComplete, name, var);
}

void Service::OnHeartbeatError(CO_Data *d, uint8_t heartbeatID)
{
    qDebug() << "Heartbeat error on node"<< (int)heartbeatID;
    std::thread t(ProcessEvents, heartbeatID, 0);
    t.detach();
}

void Service::OnSlaveBootup(CO_Data *d, uint8_t nodeId)
{
    qDebug() << "BOOT UP"<< (int)nodeId;
    std::thread t(ProcessEvents, nodeId, 1);
    t.detach();
}

void Service::ProcessEvents(int canID,int event)
{
    QString name = DataHandler::getInstance()->getDeviceNameById(canID);

    if(name.isEmpty())
    {
        return;
    }

    const char *msg = NULL;
    Actuator act(canID, "act", 0x1017);
    if (event == 1)
    {
        QThread::msleep(canID * 60);

        UNS16 time = 2000;
        act.writeSDO(0x1017, 0x00, uint16, &time, sizeof(time));

        //msg = "online";
    }
    else
    {
        // 读取状态， 如果读取失败， 则认为真的离线
        uint32_t status;
        if (act.readSDO(0x1000, 0x00, uint32, sizeof(status), &status) != 0)
        {
            //msg = "offline";
        }
        else
        {
            // QThread::msleep(200);  //延迟; //加一个延时，避免信号量阻塞未取到值
        }
    }

    // qDebug() << "process_events: " << name.c_str() << ": " << event << endl;
}

void Service::RegCallbacks(CO_Data* d)
{
    //通过回调函数监控每个动作的上报信息
    for(auto i:regs)
    {
        RegisterSetODentryCallBack(d, i.index, i.sub_index, callback);
    }

    //通过回调函数监控故障信息
    for(auto j:warning_regs)
    {
        RegisterSetODentryCallBack(d, j.index, j.sub_index, warning_callback);
    }
}

UNS32 Service::callback(CO_Data *d, const indextable *table, UNS8 bSubindex)
{
    int32_t *p = (int32_t *)table->pSubindex[bSubindex].pObject;

    int16_t can_id = (*p) >> 16;
    int16_t data = (*p) & 0xffff;
    //qDebug()<< "Call Back(" << hex << table->index << "," <<(int)bSubindex << "):" << *p << "," << data;
    auto item = find_item2(can_id, table->index, bSubindex);
    if (item != nullptr)
    {
        item->result = data;
        sem_post(item->sem);
    }

    return 0;
}

UNS32 Service::warning_callback(CO_Data *d, const indextable *table, UNS8 bSubindex)
{
    int32_t *p = (int32_t *) table->pSubindex[bSubindex].pObject;
    int16_t can_id = (*p) >> 16;
    int16_t data = (*p) & 0xffff;

    AlarmMessage msg;

    //取subindex的值，根据subindex不同值返回不同信息
    switch((int) bSubindex)
    {
        case 3:
        {
            //根据data的不同值，记录不同的信息
            if(data == 1)
            {
                //std::cout << "waring callback:" << can_id << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<传感器温度采集有故障！！！！！！" << std::endl;
                //spdlog::info("waring callback:{:d}<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<传感器温度采集有故障！！！！！！", can_id);

                msg.level = Alarm::Alarm_Warning;
                msg.errorMessage = "传感器温度采集有故障！！！！！！" + QString::number(can_id);
                msg.handleSuggest = "请联系管理员处理！";
                // SignalManager::instance()->reportErrorMessage(msg);
            }

            if(data == 2)
            {
                //std::cout << "waring callback:" << can_id << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<传感器温度差异过大！！！！！！" << std::endl;
                //spdlog::info("waring callback:{:d}<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<传感器温度差异过大！！！！！！", can_id);
                msg.level = Alarm::Alarm_Warning;
                msg.errorMessage = "传感器温度差异过大！！！！！！" + QString::number(can_id);
                msg.handleSuggest = "请联系管理员处理！";
                // SignalManager::instance()->reportErrorMessage(msg);
            }

            break;
        }
        case 5:
        {
            if(data == 2)
            {
                //std::cout << "waring callback:" << can_id << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<热盖阶段升温超时！！！！！！" << std::endl;
               // spdlog::info("waring callback:{:d}<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<热盖阶段升温超时！！！！！！", can_id);
                msg.level = Alarm::Alarm_Fatal;
                msg.errorMessage = "热盖阶段升温超时！！！！！！" + QString::number(can_id);
                msg.handleSuggest = "请联系管理员处理！";
                // SignalManager::instance()->reportErrorMessage(msg);
            }

            if(data == 3)
            {
                //std::cout << "waring callback:" << can_id << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<循环阶段控温超时！！！！！！" << std::endl;
               // spdlog::info("waring callback:{:d}<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<循环阶段控温超时！！！！！！", can_id);
                msg.level = Alarm::Alarm_Fatal;
                msg.errorMessage = "循环阶段控温超时！！！！！！" + QString::number(can_id);
                msg.handleSuggest = "请联系管理员处理！";
                // SignalManager::instance()->reportErrorMessage(msg);
            }
            if(data == 4)
            {
               // std::cout << "waring callback:" << can_id << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<冷存阶段控温超时！！！！！！" << std::endl;
               // spdlog::info("waring callback:{:d}<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<冷存阶段控温超时！！！！！！", can_id);
            }
            break;
        }
    }
}




