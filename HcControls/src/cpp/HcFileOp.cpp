#include <qdebug.h>
#include <mutex>
#include <QFile>
#include <QQmlEngine>

#include "HcFileOp.h"

HcFileOp::HcFileOp(QObject *parent) : QObject(parent)
{
}

HcFileOp *HcFileOp::getInstance()
{
    static std::once_flag of;
    static HcFileOp* instance = nullptr;

    std::call_once(of, [](){ instance = new HcFileOp(); });
    return instance;
}

HcFileOp *HcFileOp::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine);
    Q_UNUSED(jsEngine);

    return getInstance();
}

//void HcFileOp::registQML()
//{
//    qmlRegisterType<HcFileOp>("HcFileOp", 1, 0, "HcFileOp");
//    qmlRegisterSingletonInstance<HcFileOp>("HcFileOp", 1, 0, "HcFileOp", HcFileOp::create());
//}

QString HcFileOp::read(const QUrl& path, int offset, int len)
{
    QFile file(path.toLocalFile());

    if (!file.open(QIODevice::ReadOnly))
    {
        m_error = file.errorString();
        return "";
    }

    if (!file.seek(offset))
    {
        m_error = file.errorString();
        return "";
    }

    return file.read(len);
}

QString HcFileOp::readAll(const QUrl& path)
{
    QFile file(path.toLocalFile());

    if (!file.open(QIODevice::ReadOnly))
    {
        m_error = file.errorString();
        return "";
    }

    return file.readAll();
}

int HcFileOp::write(const QUrl& path, const QString& content, int offset, int len, bool isTruncate)
{
    QFile file(path.toLocalFile());
    QIODeviceBase::OpenMode mode = isTruncate ? QIODevice::WriteOnly | QIODevice::Truncate : QIODevice::WriteOnly;

    if (!file.exists())
    {
        mode = QIODevice::WriteOnly | QIODevice::NewOnly;
    }

    if (!file.open(mode))
    {
        m_error = file.errorString();
        return 0;
    }

    if (!isTruncate)
    {
        if (!file.seek(offset))
        {
            m_error = file.errorString();
            return 0;
        }
    }

    return file.write(content.toStdString().c_str(), len);
}

int HcFileOp::writeAll(const QUrl& path, const QString& content, bool isTruncate)
{
    QFile file(path.toLocalFile());
    QIODeviceBase::OpenMode mode = isTruncate ? QIODevice::WriteOnly | QIODevice::Truncate : QIODevice::WriteOnly;

    if (!file.exists())
    {
        mode = QIODevice::WriteOnly | QIODevice::NewOnly;
    }

    if (!file.open(mode))
    {
        m_error = file.errorString();
        return 0;
    }

    return file.write(content.toStdString().c_str());
}
