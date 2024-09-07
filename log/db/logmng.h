#ifndef LOGMNG_H
#define LOGMNG_H

#include "testlogtable.h"
#include "operatelogtable.h"
#include "errorlogtable.h"

#include <QDateTime>
#include <QSqlDatabase>


//获取单例指针宏
#define dLOGMNG (LogMng::instance())

class LogMng : public QObject
{
    Q_OBJECT
public:
    enum eLogType{
        eERROR_LOG = 0,
        eOPERATE_LOG,
        eTEST_LOG
    };

public:
    //获取单例指针
    static LogMng *instance();

    void initDataBase();

    //获取日志数据表模型
    QSqlTableModel* tableModel(eLogType);

    /*获取日志条数*/
    //获取全部条数
    int counts(eLogType tType);
    //获取指定条件的条数
    int specifiedCounts(eLogType tType, const QString strConstraint);

    /*日志保存*/
    //保存错误日志
    void saveErrorLog(const QString &faultInfo, const QString &level, const QString &faultCode);
    //保存操作日志
    void saveOprtLog(const QString &deviceName, const QString &detailInfo);
    //保存实验日志
    void saveTestLog(const QString &schemeName, const QString &durationTime, const QString &deviceName, const QString &finishtDateTime);

    /*清除日志*/
    void clear(eLogType tType);
    //清除全部日志
    void clearAllLog();
    //清除全部过期日志
    void clearExpiredLog();
    //清除指定的日志
    void clearSpecifiedLog(eLogType tType, const QString strConstraint);

    //更新记录的某字段数据
    void updateFiled(eLogType tType, const QString strSet, const QString strConstraint);

private:
    LogMng(QObject *parent = nullptr);
    ~LogMng();

private:
    //初始化数据库表
    void initTables();
    //清除过期的错误日志
    void clearExpiredErrorLog();
    //清除过期的操作日志
    void clearExpiredOprtLog();
    //清除过期的实验记录
    void clearExpiredTestLog();

private:
    //实验日志表
    TestLogTable *m_testTable = nullptr;
    //操作日志表
    OperateLogTable *m_operateTable = nullptr;
    //故障日志表
    ErrorLogTable *m_errorTable = nullptr;
};

#endif // LOGMNG_H
