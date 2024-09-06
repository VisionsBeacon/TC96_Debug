#ifndef ERRORLOGTABLE_H
#define ERRORLOGTABLE_H

#include "databasetable.h"

class ErrorLogTable : public DataBaseTable
{

public:
    explicit ErrorLogTable(QObject *parent = nullptr);
    ~ErrorLogTable();

    //初始化参数
    void initTableParam() override;
    //初始化模型
    void initTableModel() override;
};

#endif // ERRORLOGTABLE_H
