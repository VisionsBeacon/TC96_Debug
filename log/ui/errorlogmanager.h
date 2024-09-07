#ifndef ERRORLOGMANAGER_H
#define ERRORLOGMANAGER_H

#include "../db/logmng.h"

#include <QObject>

class ErrorLogManager : public QObject
{
    Q_OBJECT
public:
    explicit ErrorLogManager(QObject *parent = nullptr);

    //获取单例指针
    static ErrorLogManager *instance();

    //进入页面前的准备事务
    Q_INVOKABLE QVariantList enterLogBefore();

    //更新数据模型
    Q_INVOKABLE void updateModel(QString strFilter);

    //获取指定条数的记录
    Q_INVOKABLE QVariantList getRecords(int size);

private:
    QSqlTableModel *m_tableModel;
};

#endif // ERRORLOGMANAGER_H
