#include "logmng.h"

#include <QApplication>
#include <QDir>

//故障日志、操作日志、运行日志保存时间(单位: 月)
const int SAVE_DATE_MONTH = 12;
const int LOG_LIMIT = 100000;
Q_GLOBAL_STATIC_WITH_ARGS(QString, DATABASE_NAME, ("database-building.db3"))

LogMng *LogMng::instance()
{
    static LogMng instance;
    return &instance;
}

//仅在启动软件时调用一次
void LogMng::initDataBase()
{
    //用户数据库的地址
    QString logDataBasePath = QApplication::applicationDirPath() + "/LogDataBase/" + *DATABASE_NAME;

    QDir dir(QApplication::applicationDirPath() + "/LogDataBase");
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(logDataBasePath);

    //初始化表
    initTables();
}

QSqlTableModel *LogMng::tableModel(LogMng::eLogType type)
{
    QSqlTableModel *tableModel = nullptr;

    switch (type)
    {
    case eERROR_LOG:
        tableModel = m_errorTable->tableModel();
        break;
    case eOPERATE_LOG:
        tableModel = m_operateTable->tableModel();
        break;
    case eTEST_LOG:
        tableModel = m_testTable->tableModel();
        break;
    default:
        break;
    }

    return tableModel;
}

int LogMng::counts(LogMng::eLogType type)
{
    int iCount = -1;

    switch (type)
    {
    case eERROR_LOG:
        iCount = m_errorTable->counts();
        break;
    case eOPERATE_LOG:
        iCount = m_operateTable->counts();
        break;
    case eTEST_LOG:
        iCount = m_testTable->counts();
        break;
    default:
        break;
    }

    return iCount;
}

int LogMng::specifiedCounts(LogMng::eLogType type, const QString strConstraint)
{
    int iCount = -1;

    switch (type)
    {
    case eERROR_LOG:
        iCount = m_errorTable->specifiedCounts(strConstraint);
        break;
    case eOPERATE_LOG:
        iCount = m_operateTable->specifiedCounts(strConstraint);
        break;
    case eTEST_LOG:
        iCount = m_testTable->specifiedCounts(strConstraint);
        break;
    default:
        break;
    }

    return iCount;
}

//保存错误日志
void LogMng::saveErrorLog(const QString &faultInfo, const QString &level, const QString &faultCode)
{
    //清除超过一年的数据，再覆盖保存
    QSqlRecord tRecord = m_errorTable->emptyRecord();
    tRecord.setValue("FaultInfo", faultInfo);
    tRecord.setValue("Level", level);
    tRecord.setValue("FaultCode", faultCode);
    tRecord.setValue("DateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(m_errorTable->addSingleRecord(tRecord))
    {
        //判断是否超出限制条数
        int a = counts(LogMng::eERROR_LOG) - LOG_LIMIT;
        if(a > 0)
        {
            QString strConstraint = QString("DateTime IN  (SELECT  DateTime  FROM ErrorLogTable ORDER BY DateTime ASC LIMIT %1);").arg(a);
            clearSpecifiedLog(LogMng::eERROR_LOG, strConstraint);
        }
    }
}

//保存操作日志
void LogMng::saveOprtLog(const QString &deviceName, const QString &detailInfo)
{
    //清除超过一年的数据，再覆盖保存
    QSqlRecord tRecord = m_operateTable->emptyRecord();
    tRecord.setValue("DeviceName", deviceName);
    tRecord.setValue("DetailInfo", detailInfo);
    tRecord.setValue("StartDateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(m_operateTable->addSingleRecord(tRecord))
    {
        //判断是否超出限制条数
        int a = counts(LogMng::eOPERATE_LOG) - LOG_LIMIT;
        if(a > 0)
        {
            QString strConstraint = QString("DateTime IN  (SELECT  DateTime  FROM OperateLogTable ORDER BY DateTime ASC LIMIT %1);").arg(a);
            clearSpecifiedLog(LogMng::eOPERATE_LOG, strConstraint);
        }
    }
}

//保存实验日志
void LogMng::saveTestLog(const QString &schemeName, const QString &durationTime, const QString &deviceName, const QString &finishtDateTime)
{
    //清除超过一年的数据，再覆盖保存
    QSqlRecord tRecord = m_testTable->emptyRecord();
    tRecord.setValue("SchemeName", schemeName);
    tRecord.setValue("DurationTime", "-1");
    tRecord.setValue("DeviceName", deviceName);
    tRecord.setValue("StartDateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    tRecord.setValue("FinishtDateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(m_testTable->addSingleRecord(tRecord))
    {
        //判断是否超出限制条数
        int a = counts(LogMng::eTEST_LOG) - LOG_LIMIT;
        if(a > 0)
        {
            QString strConstraint = QString("DateTime IN  (SELECT  DateTime  FROM TestLogTable ORDER BY DateTime ASC LIMIT %1);").arg(a);
            clearSpecifiedLog(LogMng::eTEST_LOG, strConstraint);
        }
    }
}

void LogMng::clear(LogMng::eLogType type)
{
    switch (type)
    {
    case eERROR_LOG:
        m_errorTable->clear();
        break;
    case eOPERATE_LOG:
        m_operateTable->clear();
        break;
    case eTEST_LOG:
        m_testTable->clear();
        break;
    default:
        break;
    }
}

void LogMng::clearAllLog()
{
    m_errorTable->clear();
    m_operateTable->clear();
    m_testTable->clear();
}

//清除过期日志
void LogMng::clearExpiredLog()
{
    //清理过期操作、故障、运行日志
    clearExpiredOprtLog();
    clearExpiredErrorLog();
    clearExpiredTestLog();
}

//清除指定条件日志
void LogMng::clearSpecifiedLog(LogMng::eLogType type, const QString strConstraint)
{
    switch (type)
    {
    case eERROR_LOG:
        m_errorTable->deleteSpecifiedRecord(strConstraint);
        break;
    case eOPERATE_LOG:
        m_operateTable->deleteSpecifiedRecord(strConstraint);
        break;
    case eTEST_LOG:
        m_testTable->deleteSpecifiedRecord(strConstraint);
        break;
    default:
        break;
    }
}

LogMng::LogMng(QObject *parent)
{
    Q_UNUSED(parent)

    m_errorTable = nullptr;
    m_operateTable = nullptr;
    m_testTable = nullptr;
}

LogMng::~LogMng()
{

}

void LogMng::initTables()
{
    m_errorTable = new ErrorLogTable(this);
    if (!QSqlDatabase::database().tables().contains(m_errorTable->tableName()))
    {
        m_errorTable->creatTable();
    }

    m_errorTable->initTableModel();

    m_operateTable = new OperateLogTable(this);
    if (!QSqlDatabase::database().tables().contains(m_operateTable->tableName()))
    {
        m_operateTable->creatTable();
    }

    m_operateTable->initTableModel();

    m_testTable = new TestLogTable(this);
    if(!QSqlDatabase::database().tables().contains(m_testTable->tableName()))
    {
        m_testTable->creatTable();
    }

    m_testTable->initTableModel();
}

void LogMng::clearExpiredErrorLog()
{
    QString strConstraint =  QString("DateTime < '%1'").arg(QDate::currentDate().addMonths(-SAVE_DATE_MONTH).toString("yyyy-MM-dd"));
    m_errorTable->deleteSpecifiedRecord(strConstraint);
}

void LogMng::clearExpiredOprtLog()
{
    QString strConstraint =  QString("DateTime < '%1'").arg(QDate::currentDate().addMonths(-SAVE_DATE_MONTH).toString("yyyy-MM-dd"));
    m_operateTable->deleteSpecifiedRecord(strConstraint);
}

void LogMng::clearExpiredTestLog()
{
    QString strConstraint =  QString("DateTime < '%1'").arg(QDate::currentDate().addMonths(-SAVE_DATE_MONTH).toString("yyyy-MM-dd"));
    m_testTable->deleteSpecifiedRecord(strConstraint);
}
