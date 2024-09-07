#include "Config/registertoqml.h"
#include "log/db/logmng.h"
#include "Executor/service.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //日志数据库
    dLOGMNG->initDataBase();

    //注册C++类
    RegisterToQml::getInstance();

    //初始化Server
    Service::instance();

#ifdef QT_DEBUG
    bool isDebugMode = true;
#else
    bool isDebugMode = false;
#endif

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("isDebugMode", isDebugMode);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TC96_Debug", "Main");

    return app.exec();
}
