#include "databasetable.h"

DataBaseTable::DataBaseTable(QObject *parent)
    : QObject(parent)
{
    m_tableModel = nullptr;
}

DataBaseTable::~DataBaseTable()
{
    deleteLater();
}

bool DataBaseTable::creatTable()
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(m_strCreatTableCmd);
    bRes = query.exec();
    if (!bRes)
    {
        qDebug() << QString("Create %1 failed! Reason: ").arg(m_tableName) << query.lastError();
        qDebug() << m_strCreatTableCmd;
    }
    return bRes;
}

bool DataBaseTable::deleteTable()
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("DROP TABLE " + m_tableName));
    bRes = query.exec();
    if (!bRes)
    {
        qDebug() << QString("Delete %1 failed! Reason: ").arg(m_tableName)<<query.lastError();
    }
    return bRes;
}

int DataBaseTable::counts()
{
    int iNum = -1;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("select count(*) from " + m_tableName));
    bool bRes = query.exec();
    if (bRes)
    {
        while (query.next())
        {
            iNum = query.value(0).toInt();
        }
    }
    else
    {
        qDebug() << "GetCounts failed! Reason: "<<query.lastError();
    }
    return iNum;
}

int DataBaseTable::specifiedCounts(const QString strConstraint)
{
    int iNum = -1;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("select count(*) from " + m_tableName + " WHERE %1;").arg(strConstraint));
    bool bRes = query.exec();
    if (bRes)
    {
        while (query.next())
        {
            iNum = query.value(0).toInt();
        }
    }
    else
    {
        qDebug() << "GetSpecifiedCounts failed! Reason: "<<query.lastError();
    }
    return iNum;
}

QSqlTableModel* DataBaseTable::tableModel() const
{
    return m_tableModel;
}

QSqlRecord DataBaseTable::emptyRecord()
{
    QSqlRecord tRecord;
    if (nullptr != m_tableModel)
    {
        tRecord = m_tableModel->record();
    }
    return tRecord;
}

bool DataBaseTable::addSingleRecord(const QSqlRecord& tRecord)
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(m_addRecordCmdHead);

    for (int i = 0; i < tRecord.count(); i++)
    {
        QString fieldName = tRecord.field(i).name();
        QVariant value = tRecord.field(i).value();

        switch (tRecord.field(i).value().metaType().id())
        {
        case QMetaType::QString:
            query.bindValue(":" + fieldName, value.toString());
            break;
        case QMetaType::Int:
            query.bindValue(":" + fieldName, value.toInt());
            break;
        default:
            break;
        }
    }

    QMutexLocker tLocker(&m_mutex);
    bRes = query.exec();
    tLocker.unlock();

    if (bRes)
    {
        m_modelNeedUpdate = true;
    }
    else
    {
        qDebug() << "AddSingleRecord failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::addManyRecords(const QQueue<QSqlRecord>& tRecords)
{
    bool bRes = false;
    QSqlRecord tRecord;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(m_addRecordCmdHead);
    foreach (tRecord, tRecords)
    {
        for (int i = 0; i<tRecord.count(); i++)
        {
            QString fieldName = tRecord.field(i).name();
            QVariant value = tRecord.field(i).value();

            switch (tRecord.field(i).value().metaType().id())
            {
            case QMetaType::QString:
                query.bindValue(":" + fieldName, value.toString());
                break;
            case QMetaType::Int:
                query.bindValue(":" + fieldName, value.toInt());
                break;
            default:
                break;
            }
        }

        QMutexLocker tLocker(&m_mutex);
        bRes = query.exec();
        tLocker.unlock();
    }
    if (bRes)
    {
        m_modelNeedUpdate = true;
    }
    else
    {
        qDebug()<<"AddManyRecords failed! Reason: "<<query.lastError();
    }
    return bRes;
}

bool DataBaseTable::deleteSpecifiedRecord(const QString strConstraint)
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("DELETE from " + m_tableName + " WHERE %1").arg(strConstraint));
    bRes = query.exec();
    if (bRes)
    {
        m_modelNeedUpdate = true;
    }
    else
    {
        qDebug() << "DeleteSpecifiedRecord failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::clear()
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare( QString("DELETE from " + m_tableName));
    bRes = query.exec();
    if (bRes)
    {
        m_modelNeedUpdate = true;
    }
    else
    {
        qDebug() << "Clear failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::updateField(const QString strSet, const QString strConstraint)
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("UPDATE " + m_tableName + " SET %1 WHERE %2").arg(strSet).arg(strConstraint));
    bRes = query.exec();
    if (bRes)
    {
        m_modelNeedUpdate = true;
    }
    else
    {
        qDebug() << "UpdateField failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::queryAllRecords(QList<QSqlRecord> &RecordQueue)
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("SELECT * FROM " + m_tableName));
    bRes = query.exec();
    if (bRes)
    {
        while(query.next())
        {
            RecordQueue << query.record();
        }
    }
    else
    {
        qDebug() << "GetAllRecords failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::querySpecifiedRecords(QList<QSqlRecord> &RecordQueue, const QString strConstraint)
{
    bool bRes = false;
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(QString("SELECT * FROM " + m_tableName + " WHERE %1;").arg(strConstraint));
    QMutexLocker tLocker(&m_mutex);
    bRes = query.exec();
    tLocker.unlock();
    if (bRes)
    {
        while(query.next())
        {
            RecordQueue << query.record();
        }
    }
    else
    {
        qDebug() << "GetSpecifiedRecords failed! Reason: " << query.lastError();
    }
    return bRes;
}

bool DataBaseTable::existField(const QSqlField tField)
{
    bool bRes = false;
    QList<QSqlRecord> tRecordsQueue;
    querySpecifiedRecords(tRecordsQueue, QString("%1 = '%2'").arg(tField.name()).arg(tField.value().toString()));
    if(tRecordsQueue.count() != 0)
    {
        bRes = true;
    }
    return bRes;
}

void DataBaseTable::initTableRecords()
{

}

QString DataBaseTable::tableName() const
{
    return m_tableName;
}
