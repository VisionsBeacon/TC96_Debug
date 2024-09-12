#include "service.h"
#include "signalmanager.h"
#include "datahandler.h"
#include "acturator.h"
#include "waiting_list.h"
#include "executorfactory.h"


#include <QRandomGenerator>
#include <QThread>

// #include <QtConcurrent/QtConcurrent>


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
    init();
}

Service *Service::instance()
{
    if(!m_Ctrl)
    {
        m_Ctrl = new Service();
    }

    return m_Ctrl;
}

void Service::init()
{
    initConnect();
}

void Service::initConnect()
{
    connect(sigManager, &SignalManager::sigStartLanServer, this, &Service::onSigStartLanServer);
    connect(sigManager, &SignalManager::sigSendNormalCanCommand, this, &Service::onSigSendNormalCanCommand);
}

//初始化
void Service::CanopenInit()
{
    TimerInit();
    InitSdoClients();

    board.busname = (char *)DataHandler::getInstance()->zlg_can_ip().toStdString().c_str();

    if(canOpen(&board, &eds_master_Data) == 0)
    {
        //打开Lan服务失败
        qDebug() << "打开Lan服务失败";
        sigManager->sigResultStartLanServer(false);
        return;
    }

    //启动定时器
    eds_master_Data.heartbeatError = OnHeartbeatError;
    eds_master_Data.post_SlaveBootup = OnSlaveBootup;
    RegCallbacks(&eds_master_Data);
    StartTimerLoop(NULL);
    QThread::sleep(1);

    setNodeId(&eds_master_Data, 0x01);
    setState(&eds_master_Data, Initialisation);

    //打开Lan服务成功
    qDebug() << "打开Lan服务成功";
    sigManager->sigResultStartLanServer(true);
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
    QString can_id = param["can_id"].toString();
    QString name = DataHandler::getInstance()->getDeviceNameById(can_id.toInt());

    QString actionName = "";
    QString excuteResult = "成功";

    switch(funName)
    {
    case FUN_RESET:
    {
        actionName = "复位";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "reset");

        if (e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_START:
    {
        actionName = "方案运行";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "start");

        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }
        break;
    }
    case FUN_STOP:
    {
        actionName = "方案停止";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "stop");

        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_LID_OPEN:
    {
        actionName = "开盖";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "lid_open");

        if (e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_LID_CLOSE:
    {
        actionName = "关盖";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "lid_close");

        if(e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }

        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_HEAT_LID_ON:
    {
        break;
    }
    case FUN_HEAT_LID_OFF:
    {
        break;
    }
    case FUN_SET_RDML:
    {
        actionName = "方案设置";
        auto e = ExecutorFactory::Instance().CreateExecutorWrapper(name, "set_schema");

        if (e != nullptr)
        {
            if(e->run(param["param"].toString()) != 0)
            {
                excuteResult = "失败";
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_SET_PARAM:
    {
        actionName = "设置参数";

        auto e1 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "heatlidpid");
        auto e2 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "blockpid");
        auto e3 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "basicparam");
        auto e5 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "calibratecompensation");
        auto e6 = ExecutorFactory::Instance().CreateExecutorWrapper(name, "cyclingcompensation");

        //取出需要写的值
        QString paramStr = param["param"].toString();
        QJsonDocument paramDoc = QJsonDocument::fromJson(paramStr.toUtf8());
        QJsonObject paramObj = paramDoc.object();

        //热盖PID
        int32_t heatlid_kp = paramObj["heatlid_kp"].toInt();
        int32_t heatlid_ki = paramObj["heatlid_ki"].toInt();
        int32_t heatlid_kd = paramObj["heatlid_kd"].toInt();

        //加热块PID
        int32_t block_kp = paramObj["block_kp"].toInt();
        int32_t block_kd = paramObj["block_kd"].toInt();
        int32_t block_ki = paramObj["block_ki"].toInt();

        //热盖距离
        double lid_move_distance = paramObj["lid_distance"].toString().toDouble();

        //传感器温度补偿、热盖温度补偿
        int32_t block1 = paramObj["block1"].toInt();
        int32_t block2 = paramObj["block2"].toInt();
        int32_t block3 = paramObj["block3"].toInt();
        int32_t heatLid = paramObj["heatLid"].toInt();

        //温控补偿
        int32_t k = paramObj["k"].toInt();
        int32_t b = paramObj["b"].toInt();

        //热盖PID
        if (e1 != nullptr)
        {
            if (e1->write_data(0, heatlid_kp * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e1->write_data(1, heatlid_ki * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e1->write_data(2, heatlid_kd * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        //加热块PID
        if (e2 != nullptr)
        {
            if (e2->write_data(0, block_kp * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e2->write_data(1, block_ki * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e2->write_data(2, block_kd * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        //热盖距离
        if(e3 != nullptr)
        {
            if(e3->write_data(0, lid_move_distance * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        //传感器温度补偿、热盖温度补偿
        if (e5 != nullptr) {
            if (e5->write_data(0, block1 * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
            if (e5->write_data(1, block2 * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e5->write_data(2, block3 * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
            if (e5->write_data(3, heatLid * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        //温控补偿
        if (e6 != nullptr)
        {
            if (e6->write_data(0, k * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }

            if (e6->write_data(1, b * 1000) != 0)
            {
                excuteResult = "失败";
            }
            else
            {
                QThread::msleep(200);
            }
        }
        else
        {
            excuteResult = "设备不在线";
        }

        break;
    }
    case FUN_READ_PARAM:
    {
        actionName = "获取参数";
        excuteResult = "结果";
        getParams(name);

        //这里为return是为了暂时过滤获取参数结果，没什么好返回的
        return;
    }
    case FUN_TEMPERATURE:
    {
        actionName = "温度";
        excuteResult = "结果";
        getTemperature(name);

        //这里为return是为了暂时过滤温度结果，没什么好返回的
        return;
    }
    default:
        break;
    }

    Q_EMIT sigManager->sendActionResult(actionName + excuteResult);
}

//读取温度
void Service::getTemperature(const QString &deviceName)
{
    //读取温度值
    int32_t valueTemperature1 = 8888;
    int32_t valueTemperature2 = 8888;
    int32_t valueTemperature3 = 8888;
    int32_t valueTemperature4 = 8888;
    int32_t valueTemperature5 = 8888;

    //创建了一个Temperatures类
    auto e = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "temperature");

    if (e != nullptr)
    {
        // //用于自测的随机温度值
        // QRandomGenerator *randomGenerator = QRandomGenerator::global();

        // // 生成随机温度值，范围在20到100摄氏度之间
        // valueTemperature1 = randomGenerator->bounded(20000, 100000);
        // valueTemperature2 = randomGenerator->bounded(20000, 100000);
        // valueTemperature3 = randomGenerator->bounded(20000, 100000);
        // valueTemperature4 = randomGenerator->bounded(20000, 100000);
        // valueTemperature5 = randomGenerator->bounded(20000, 100000);

        if(e->read_data(0, valueTemperature1) != 0)
        {
            //获取失败
        }
        else
        {
            //加一个延时，避免信号量阻塞未取到值
            QThread::msleep(200);
        }

        if(e->read_data(1, valueTemperature2) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if(e->read_data(2, valueTemperature3) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if(e->read_data(3, valueTemperature4) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if(e->read_data(4, valueTemperature5) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {

    }

    //处理一些异常数据
    if(valueTemperature1 == 8888)
    {
        valueTemperature1 = valueTemperature2;
    }
    if(valueTemperature2 == 8888)
    {
        valueTemperature2 = valueTemperature3;
    }

    QVariantList vec_res;
    vec_res.append(valueTemperature1 / 1000);
    vec_res.append(valueTemperature2 / 1000);
    vec_res.append(valueTemperature4 / 1000);
    vec_res.append(valueTemperature3 / 1000);
    vec_res.append(valueTemperature5 / 1000);

    Q_EMIT sigManager->sigSendTemperature(deviceName, vec_res);
}

//获取参数
void Service::getParams(const QString &deviceName)
{
    Data data;

    auto e1 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "heatlidpid");
    auto e2 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "blockpid");
    auto e3 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "basicparam");
    auto e4 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "canparam");
    auto e5 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "calibratecompensation");
    auto e6 = ExecutorFactory::Instance().CreateExecutorWrapper(deviceName, "cyclingcompensation");

    if(e1 != nullptr)
    {
        if(e1->read_data(0, data.heatlid_kp) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
        if(e1->read_data(1, data.heatlid_ki) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if(e1->read_data(2, data.heatlid_kd) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    if (e2 != nullptr)
    {
        if(e2->read_data(0, data.block_kp) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
        if(e2->read_data(1, data.block_ki) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if(e2->read_data(2, data.block_kd) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    if (e3 != nullptr)
    {
        if (e3->read_data(0, data.lid_move_distance) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    if (e4 != nullptr)
    {
        if (e4->read_data(0, data.can_id) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e4->read_data(1, data.baundrate) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    if (e5 != nullptr)
    {
        if (e5->read_data(0, data.block1) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e5->read_data(1, data.block2) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e5->read_data(2, data.block3) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e5->read_data(3, data.heatLid) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e5->read_data(4, data.radiator) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    if (e6 != nullptr)
    {
        if (e6->read_data(0, data.k) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }

        if (e6->read_data(1, data.b) != 0)
        {
            //获取失败
        }
        else
        {
            QThread::msleep(200);
        }
    }
    else
    {
        //设备不在线
    }

    Q_EMIT sigManager->sigSendParams(deviceName, data);
}

//开启can服务
void Service::onSigStartLanServer()
{
    CanopenInit();
}

//接收can指令
void Service::onSigSendNormalCanCommand(int commandType, const QJsonObject &obj)
{
    auto future = QtConcurrent::run([this, commandType, obj]() {
        InterfaceComplete(static_cast<FUNNAME>(commandType), obj);
    });
}

void Service::OnHeartbeatError(CO_Data *d, uint8_t heartbeatID)
{
    qDebug() << "Heartbeat error on node" << (int)heartbeatID;
    std::thread t(ProcessEvents, heartbeatID, 0);
    t.detach();
}

void Service::OnSlaveBootup(CO_Data *d, uint8_t nodeId)
{
    qDebug() << "BOOT UP" << (int)nodeId;
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
            QThread::msleep(200);  //延迟; //加一个延时，避免信号量阻塞未取到值
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
        // sem_post(item->sem);
        item->sem->release();
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




