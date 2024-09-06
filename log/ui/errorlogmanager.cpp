#include "errorlogmanager.h"

const int SAMPLE_SHOW_PAGE_NUM = 10;

ErrorLogManager::ErrorLogManager(QObject *parent)
    : QObject{parent}
{
    m_tableModel = dLOGMNG->tableModel(LogMng::eERROR_LOG);
}

ErrorLogManager *ErrorLogManager::instance()
{
    static ErrorLogManager instance;
    return &instance;
}

//进入页面前的准备事务
QVariantList ErrorLogManager::enterLogBefore()
{
    updateModel("");

    return getRecords(SAMPLE_SHOW_PAGE_NUM);
}

//更新数据模型
void ErrorLogManager::updateModel(QString strFilter)
{
    //设置筛选条件
    if (!strFilter.isEmpty())
    {
        m_tableModel->setFilter(strFilter);
    }

    //从数据库获取满足筛选条件的数据
    m_tableModel->select();
    while (m_tableModel->canFetchMore())
    {
        m_tableModel->fetchMore();
    }
}

//获取指定条数的记录
QVariantList ErrorLogManager::getRecords(int size)
{
    QVariantList list;
    QQueue<QSqlRecord> tRecords;
    QSqlRecord tRecord;

    while (0 != size)
    {
        --size;
        tRecord = m_tableModel->record(size);
        if (!tRecord.isNull(0))
        {
            tRecords << tRecord;
        }
    }

    if (0 == tRecords.size())
    {
        return list;
    }

    for (int iRow = 0; iRow < tRecords.size(); ++iRow)
    {
        QVariantMap map;
        tRecord = tRecords.at(tRecords.size() - iRow - 1);

        map["FaultInfo"] = tRecord.value("FaultInfo").toString();
        map["Level"] = tRecord.value("Level").toString();
        map["FaultCode"] = tRecord.value("FaultCode").toString();
        map["DateTime"] = tRecord.value("DateTime").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        list.append(map);
    }

    return list;
}
