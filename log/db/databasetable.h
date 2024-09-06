#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QObject>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QSqlTableModel>
#include <QQueue>
#include <QMutex>
#include <QDebug>

class DataBaseTable : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseTable(QObject *parent = nullptr);
    virtual ~DataBaseTable();

public:
    //建表
    bool creatTable();
    //删除表
    bool deleteTable();
    //获取表名
    QString tableName() const;
    //获取表的模型
    QSqlTableModel *tableModel() const;
    //获取表的一条空记录
    QSqlRecord emptyRecord();
    //获取全部记录条数
    int counts();
    //获取指定条件的记录条数
    int specifiedCounts(const QString strConstraint);
    //增加单条记录
    bool addSingleRecord(const QSqlRecord& tRecord);
    //增加多条记录
    bool addManyRecords(const QQueue<QSqlRecord>& tRecords);
    //清除表的全部记录
    bool clear();
    //删除指定条件的记录
    bool deleteSpecifiedRecord(const QString strConstraint);
    /*更改记录某字段*/
    bool updateField(const QString strSet, const QString strConstraint);
    /*判断某字段是否存在*/
    bool existField(const QSqlField tField);
    //查询全部记录
    bool queryAllRecords(QList<QSqlRecord>& RecordQueue);
    //查询指定记录
    bool querySpecifiedRecords(QList<QSqlRecord>& RecordQueue, const QString strConstraint);

    //初始化表的参数
    virtual void initTableParam() = 0;
    //初始化表的数据模型
    virtual void initTableModel() = 0;
    //初始化表的记录
    virtual void initTableRecords();

public:
    //表名
    QString m_tableName;
    //添加记录的命令头
    QString m_addRecordCmdHead;
    //创建表的命令
    QString m_strCreatTableCmd;
    //是否需要更新模型
    volatile bool m_modelNeedUpdate;
    //表模型
    QSqlTableModel *m_tableModel;
    //互斥体
    QMutex m_mutex;
};

#endif // DATABASETABLE_H
