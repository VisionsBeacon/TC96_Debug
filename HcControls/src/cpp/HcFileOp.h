#ifndef HC_FILE_OP_H
#define HC_FILE_OP_H

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

class HcFileOp : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(HcFileOp)
    QML_NAMED_ELEMENT(HcFileOp)
    QML_SINGLETON

public:
    static HcFileOp* getInstance();
    static HcFileOp* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    // 保留，方便测试程序注册qml类型，其它c++类型类似，不再赘述
//    static void registQML();

    Q_INVOKABLE QString read(const QUrl& path, int offset, int len);
    Q_INVOKABLE QString readAll(const QUrl& path);
    Q_INVOKABLE int write(const QUrl& path, const QString& content, int offset, int len, bool isTruncate = true);
    Q_INVOKABLE int writeAll(const QUrl& path, const QString& content, bool isTruncate = true);
    Q_INVOKABLE QString getLastEorror() const { return m_error; }   // 保存的上一次错误信息，失败调用

private:
    explicit HcFileOp(QObject *parent = nullptr);

private:
    QString m_error;
};

#endif // HC_FILE_OP_H
