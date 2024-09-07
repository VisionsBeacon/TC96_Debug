#ifndef OPERATELOGMANAGER_H
#define OPERATELOGMANAGER_H

#include "../db/logmng.h"

#include <QObject>

class OperateLogManager : public QObject
{
    Q_OBJECT
public:
    explicit OperateLogManager(QObject *parent = nullptr);

    //获取单例指针
    static OperateLogManager *instance();

    //进入页面前的准备事务
    Q_INVOKABLE QVariantList enterLogBefore();

    //更新数据模型
    Q_INVOKABLE void updateModel(QString strFilter);

    //获取指定条数的记录
    Q_INVOKABLE QVariantList getRecords(int size);

    //新增一条记录
    Q_INVOKABLE void addNewOperationRecord(const QString &deviceName, const QString &detailInfo);

private:
    QSqlTableModel *m_tableModel;
};

#endif // OPERATELOGMANAGER_H
