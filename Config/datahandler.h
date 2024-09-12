#ifndef DATAHANDLER_H
#define DATAHANDLER_H
#include "signalmanager.h"

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QVariant>

class DataHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap m_deviceMap READ getDeviceMap)

public:
    explicit DataHandler(QObject *parent = nullptr);
    void init();
    void initConnect();
    static DataHandler *getInstance();

    /***************************QML接口函数***************************/

    //加载配置文件
    Q_INVOKABLE void loadingDevicesConfig();
    //获取设备名称与id的map
    Q_INVOKABLE QVariantMap getDeviceMap() const;
    //开启Lan服务
    Q_INVOKABLE void startLanServer();
    //发送Can指令
    Q_INVOKABLE void sendCommand(int canId, int commandType, const QString &paramsJson);


    /***************************正常函数***************************/

    //解析配置文件
    bool parseDevicesConfig();
    //根据设备名称返回id
    int getCanIdForDevice(const QString &name);
    //根据设备id返回名称
    QString getDeviceNameById(int id);
    QString zlg_can_ip() const;

signals:
    //解析配置文件完成
    void parseDevicesConfigCompleted(bool);
    //打开Lan服务结果
    void sigResultStartLanServer(bool);
    //发送动作执行结果给UI
    void sendActionResult(const QString &msg);
    //发送温度
    void sigSendTemperature(const QString &deviceName, const QVariantList &temperatureList);
    //发送参数
    void sigSendParams(const QString &deviceName, const Data &data);

private slots:
    void onFinished();
    //接收开启Lan服务结果
    void onSigResultStartLanServer(bool result);
    //接收动作执行结果
    void onSendActionResult(const QString &msg);
    //接收温度
    void onSigSendTemperature(const QString &deviceName, const QVariantList &temperatureList);
    //接收参数
    void onSigSendParams(const QString &deviceName, const Data &data);


private:
    QFutureWatcher<bool> m_watcher;
    static DataHandler *m_dataHandler;
    QVariantMap m_deviceMap;    //存储每一对设备的can_id和名称
    QString m_zlg_can_ip;
};

#endif // DATAHANDLER_H
