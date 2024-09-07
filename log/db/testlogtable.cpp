#include "testlogtable.h"

TestLogTable::TestLogTable(QObject *parent)
    :DataBaseTable(parent)
{
    initTableParam();
}

TestLogTable::~TestLogTable()
{
    deleteLater();
}

void TestLogTable::initTableModel()
{
    if(m_tableModel != nullptr)
    {
        m_tableModel->clear();
    }
    m_tableModel = new QSqlTableModel(this, QSqlDatabase::database());
    m_tableModel->setTable(m_tableName);
    //设置模型提交策略，手动提交
    m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //填充模型数据
    if (!m_tableModel->select())
    {
        qDebug() << QString("fill model %1 data failed").arg(m_tableName);
    }
    //排序
    m_tableModel->setSort(m_tableModel->fieldIndex("StartDateTime"), Qt::DescendingOrder);
    m_modelNeedUpdate = false;
}

void TestLogTable::initTableParam()
{
    m_tableName = QString("TestLogTable");

    m_addRecordCmdHead = QString(
        "INSERT INTO " + m_tableName +
        " (SchemeName, DurationTime, DeviceName, StartDateTime, FinishtDateTime)"
        "VALUES(:SchemeName, :DurationTime, :DeviceName, :StartDateTime, :FinishtDateTime)");

    m_strCreatTableCmd = QString(
        "CREATE TABLE IF NOT EXISTS " + m_tableName +
        " ("
        "SchemeName         VARCHAR      NULL,"
        "DurationTime       VARCHAR      NULL,"
        "DeviceName         VARCHAR      NULL,"
        "StartDateTime      QDateTime    NULL,"
        "FinishtDateTime    QDateTime    NULL"
        ")");
}
