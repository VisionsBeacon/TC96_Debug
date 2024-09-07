#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#define sigManager SignalManager::instance()

#include <QObject>

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

private:
    static SignalManager* m_signalManager;

};

#endif // SIGNALMANAGER_H
