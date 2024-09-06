#include "errorlogtable.h"

ErrorLogTable::ErrorLogTable(QObject *parent)
    : DataBaseTable{parent}
{
    initTableParam();
}

ErrorLogTable::~ErrorLogTable()
{
    deleteLater();
}

void ErrorLogTable::initTableModel()
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
    m_tableModel->setSort(m_tableModel->fieldIndex("DateTime"), Qt::DescendingOrder);
    m_modelNeedUpdate = false;
}

void ErrorLogTable::initTableParam()
{
    m_tableName = QString("ErrorLogTable");

    m_addRecordCmdHead = QString(
        "INSERT INTO " + m_tableName +
        " (FaultInfo, Level, FaultCode, DateTime)"
        "VALUES(:FaultInfo, :Level, :FaultCode, :DateTime)");

    m_strCreatTableCmd = QString(
        "CREATE TABLE IF NOT EXISTS " + m_tableName +
        " ("
        "FaultInfo      VARCHAR      NULL,"
        "Level          VARCHAR      NULL,"
        "FaultCode      VARCHAR      NULL,"
        "DateTime       QDateTime    NULL"
        ")");
}
