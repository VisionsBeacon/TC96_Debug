#ifndef DATAHANDLER_H
#define DATAHANDLER_H

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
    static DataHandler *getInstance();

    //加载配置文件
    Q_INVOKABLE void loadingDevicesConfig();

    //获取设备名称与id的map
    Q_INVOKABLE QVariantMap getDeviceMap() const;

    //开启Lan服务
    Q_INVOKABLE void startLanServer();

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

    //开启Lan服务
    void sigStartLanServer();


private slots:
    void onFinished();


private:
    QFutureWatcher<bool> m_watcher;
    static DataHandler *m_dataHandler;
    QVariantMap m_deviceMap;    //存储每一对设备的can_id和名称
    QString m_zlg_can_ip;
};

#endif // DATAHANDLER_H
