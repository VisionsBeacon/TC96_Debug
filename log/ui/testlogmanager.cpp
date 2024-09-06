#include "testlogmanager.h"

const int SAMPLE_SHOW_PAGE_NUM = 10;

TestLogManager::TestLogManager(QObject *parent)
    : QObject{parent}
{
    m_tableModel = dLOGMNG->tableModel(LogMng::eTEST_LOG);
}

TestLogManager *TestLogManager::instance()
{
    static TestLogManager instance;
    return &instance;
}

//进入页面前的准备事务
QVariantList TestLogManager::enterLogBefore()
{
    updateModel("");

    return getRecords(SAMPLE_SHOW_PAGE_NUM);
}

//更新数据模型
void TestLogManager::updateModel(QString strFilter)
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
QVariantList TestLogManager::getRecords(int size)
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

        map["SchemeName"] = tRecord.value("SchemeName").toString();
        map["DurationTime"] = tRecord.value("DurationTime").toString();
        map["DeviceName"] = tRecord.value("DeviceName").toString();
        map["StartDateTime"] = tRecord.value("StartDateTime").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        map["FinishtDateTime"] = tRecord.value("FinishtDateTime").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        list.append(map);
    }

    return list;
}
