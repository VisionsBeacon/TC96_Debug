#include "registertoqml.h"
#include "signalmanager.h"
#include "datahandler.h"
#include "../log/ui/testlogmanager.h"
#include "../log/ui/operatelogmanager.h"
#include "../log/ui/errorlogmanager.h"

#include <QQmlEngine>

RegisterToQml *RegisterToQml::m_registerToQml = nullptr;

RegisterToQml::RegisterToQml(QObject *parent)
    : QObject{parent}
{
    //初始化C++类
    sigManager;

    //注册结构体
    qRegisterMetaType<Data>("Data");

    //注册C++类
    qmlRegisterSingletonInstance<DataHandler>("DataHandler", 1, 0, "DataHandler", DataHandler::getInstance());

    //注册日志管理类，共三个，通过LogMng类获取到tableModel，通过LogMng类操作数据库
    qmlRegisterSingletonInstance<TestLogManager>("TestLogManager", 1, 0, "TestLogManager", TestLogManager::instance());
    qmlRegisterSingletonInstance<OperateLogManager>("OperateLogManager", 1, 0, "OperateLogManager", OperateLogManager::instance());
    qmlRegisterSingletonInstance<ErrorLogManager>("ErrorLogManager", 1, 0, "ErrorLogManager", ErrorLogManager::instance());
}

RegisterToQml *RegisterToQml::getInstance()
{
    if(!m_registerToQml)
    {
        m_registerToQml = new RegisterToQml();
    }

    return m_registerToQml;
}
