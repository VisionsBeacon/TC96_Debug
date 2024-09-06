#include "Config/registertoqml.h"
#include "log/db/logmng.h"

#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //日志数据库
    dLOGMNG->initDataBase();

    //注册C++类
    RegisterToQml::getInstance();

    //启用Can服务


    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TC96_Debug", "Main");

    return app.exec();
}
