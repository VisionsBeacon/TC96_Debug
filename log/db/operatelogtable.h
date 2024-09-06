#ifndef OPERATELOGTABLE_H
#define OPERATELOGTABLE_H

#include "databasetable.h"


class OperateLogTable : public DataBaseTable
{

public:
    explicit OperateLogTable(QObject *parent = nullptr);
    ~OperateLogTable();

    //初始化参数
    void initTableParam() override;
    //初始化模型
    void initTableModel() override;
};

#endif // OPERATELOGTABLE_H
