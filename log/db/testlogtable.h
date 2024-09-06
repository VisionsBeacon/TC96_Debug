#ifndef TESTLOGTABLE_H
#define TESTLOGTABLE_H

#include "databasetable.h"


class TestLogTable : public DataBaseTable
{

public:
    explicit TestLogTable(QObject *parent = nullptr);
    ~TestLogTable();

    //初始化参数
    void initTableParam() override;
    //初始化模型
    void initTableModel() override;

};

#endif // TESTLOGTABLE_H
