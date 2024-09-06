#pragma once
#include <semaphore.h>

#include <QObject>


class ExecutorWrapper : public QObject
{
    Q_OBJECT
public:
    ExecutorWrapper(int can_id, const QString &name, const QString &request, QObject *parent = nullptr)
        : QObject{parent}, can_id(can_id), name(name), request(request)
    {
        sem_init(&(this->sem), 0, 0);
    }

    virtual int run(const QString &args);       //运行控制命令
    int  write_param(int param, int32_t value);      //写参数
    int read_param(int param, int32_t &value);       //读参数

    virtual int write_data(int paramName,int32_t value)
    {
        return 0;
    }

    virtual int read_data(int paramName,int32_t &value)
    {
        return 0;
    }

protected:
    QString name;
    QString request;
    sem_t sem;

protected:
    int index;
    int control;
    int status;
    int can_id;

    bool in_process();
    void set_processing();
    int set_finished();
    int wait_result();
};
