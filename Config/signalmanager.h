#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#define sigManager SignalManager::instance()

#include <QObject>

//参数
struct Data {

    Q_GADGET
    Q_PROPERTY(int32_t can_id MEMBER can_id)
    Q_PROPERTY(int32_t heatlid_kp MEMBER heatlid_kp)
    Q_PROPERTY(int32_t heatlid_ki MEMBER heatlid_ki)
    Q_PROPERTY(int32_t heatlid_kd MEMBER heatlid_kd)
    Q_PROPERTY(int32_t block_kp MEMBER block_kp)
    Q_PROPERTY(int32_t block_ki MEMBER block_ki)
    Q_PROPERTY(int32_t block_kd MEMBER block_kd)
    Q_PROPERTY(int32_t lid_move_distance MEMBER lid_move_distance)
    Q_PROPERTY(int32_t baundrate MEMBER baundrate)
    Q_PROPERTY(int32_t block1 MEMBER block1)
    Q_PROPERTY(int32_t block2 MEMBER block2)
    Q_PROPERTY(int32_t block3 MEMBER block3)
    Q_PROPERTY(int32_t heatLid MEMBER heatLid)
    Q_PROPERTY(int32_t radiator MEMBER radiator)
    Q_PROPERTY(int32_t k MEMBER k)
    Q_PROPERTY(int32_t b MEMBER b)

public:
    int32_t can_id = -1;
    int32_t heatlid_kp = -1;
    int32_t heatlid_ki = -1;
    int32_t heatlid_kd = -1;
    int32_t block_kp = -1;
    int32_t block_ki = -1;
    int32_t block_kd = -1;
    int32_t lid_move_distance = -1;
    int32_t baundrate = -1;
    int32_t block1 = -1;
    int32_t block2 = -1;
    int32_t block3 = -1;
    int32_t heatLid = -1;
    int32_t radiator = -1;
    int32_t k = -1;
    int32_t b = -1;
};
Q_DECLARE_METATYPE(Data)



class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);
    static SignalManager* instance();

signals:
    //开启Lan服务
    void sigStartLanServer();
    //返回开启Lan服务结果
    void sigResultStartLanServer(bool);
    //发送can指令
    void sigSendNormalCanCommand(int commandType, const QJsonObject &obj);
    //发送动作执行结果
    void sendActionResult(const QString &msg);
    //发送温度
    void sigSendTemperature(const QString &deviceName, const QVariantList &temperatureList);
    //发送参数
    void sigSendParams(const QString &deviceName, const Data &data);


private:
    static SignalManager* m_signalManager;

};

#endif // SIGNALMANAGER_H
