#include "operatelogtable.h"

OperateLogTable::OperateLogTable(QObject *parent)
    : DataBaseTable{parent}
{
    initTableParam();
}

OperateLogTable::~OperateLogTable()
{
    deleteLater();
}

void OperateLogTable::initTableModel()
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

void OperateLogTable::initTableParam()
{
    m_tableName = QString("OperateLogTable");

    m_addRecordCmdHead = QString(
        "INSERT INTO " + m_tableName +
        " (DeviceName, DetailInfo, StartDateTime)"
        "VALUES(:DeviceName, :DetailInfo, :StartDateTime)");

    m_strCreatTableCmd = QString(
        "CREATE TABLE IF NOT EXISTS " + m_tableName +
        " ("
        "DeviceName         VARCHAR      NULL,"
        "DetailInfo         VARCHAR      NULL,"
        "StartDateTime      QDateTime    NULL"
        ")");
}
