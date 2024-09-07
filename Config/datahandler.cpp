#include "datahandler.h"
#include "signalmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

DataHandler* DataHandler::m_dataHandler = nullptr;

DataHandler::DataHandler(QObject *parent)
    : QObject{parent}
{
    init();
}


/***************************正常函数***************************/

void DataHandler::init()
{
    initConnect();
}

void DataHandler::initConnect()
{
    connect(&m_watcher, &QFutureWatcher<bool>::finished, this, &DataHandler::onFinished);
    connect(sigManager, &SignalManager::sigResultStartLanServer, this, &DataHandler::onSigResultStartLanServer);
}

DataHandler *DataHandler::getInstance()
{
    if(!m_dataHandler)
    {
        m_dataHandler = new DataHandler();
    }

    return m_dataHandler;
}

//解析配置文件
bool DataHandler::parseDevicesConfig()
{
    m_deviceMap.clear();
    m_zlg_can_ip.clear();

    //获取可执行文件所在的目录
    QString exePath = QCoreApplication::applicationDirPath();

    QString configFile = QDir(exePath).filePath("device_config.json");

    QFile file(configFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "配置文件打开失败：" << configFile;
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull())
    {
        qWarning() << "解析配置文件失败";
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();

    QJsonArray devices = rootObj["devices"].toArray();
    for(const QJsonValue& deviceVal : devices)
    {
        QJsonObject deviceObj = deviceVal.toObject();
        QString name = deviceObj["name"].toString();
        int can_id = deviceObj["can_id"].toInt();

        m_deviceMap.insert(name, can_id);
    }

    m_zlg_can_ip = rootObj["zlg_can_ip"].toString();

    return true;
}

QString DataHandler::zlg_can_ip() const
{
    return m_zlg_can_ip;
}

int DataHandler::getCanIdForDevice(const QString &name)
{
    return m_deviceMap["name"].toInt();
}

//根据设备id返回名称
QString DataHandler::getDeviceNameById(int id)
{
    for (auto it = m_deviceMap.begin(); it != m_deviceMap.end(); ++it)
    {
        if(id == it.value().toInt())
        {
            return it.key();
        }
    }

    return "";
}


/***************************QML接口函数***************************/


//解析配置文件
void DataHandler::loadingDevicesConfig()
{
    QFuture<bool> future = QtConcurrent::run(&DataHandler::parseDevicesConfig, this);
    m_watcher.setFuture(future);
}

QVariantMap DataHandler::getDeviceMap() const
{
    return m_deviceMap;
}

//开启Lan服务
void DataHandler::startLanServer()
{
    Q_EMIT sigManager->sigStartLanServer();
}

//发送Can指令
void DataHandler::sendCommand(int canId, int commandType, const QString &rdmlJson)
{
    if(rdmlJson.isEmpty())
    {
        //不发送方案的情况
        QJsonObject jsonObject;
        jsonObject.insert("can_id", QString::number(canId));
        jsonObject.insert("param", "");

        Q_EMIT sigManager->sigSendNormalCanCommand(commandType, jsonObject);
    }
    else
    {
        //发送方案

    }

}




/***************************槽函数***************************/


//接收读取配置文件结果
void DataHandler::onFinished()
{
    // Q_EMIT parseDevicesConfigCompleted(m_watcher.result());
}

//接收开启Lan服务结果
void DataHandler::onSigResultStartLanServer(bool result)
{
    Q_EMIT sigResultStartLanServer(result);
}
