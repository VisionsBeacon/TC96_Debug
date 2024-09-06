// 包含一次
#pragma once

#include <QObject>

#include "acturator_args.h"

#include <semaphore.h>
#include <memory>
#include <QMutex>
#include <vector>

#include <QDebug>

class Actuator : public QObject
{
    // Q_OBJECT
public:
    Actuator(int can_id, const QString &name = "None", int index = 0x2600, QObject *parent = nullptr)
        : QObject(parent), can_id(can_id), name(name), index(index)
    {

    }

    virtual int writeSDO(int index, int subindex, int type, void *value, int size) final;

    virtual int setArgs(std::vector<args_item *> &value)
    {
        for (auto i : value)
        {
            qDebug() << this->name << " setArgs" << i->data();
        }

        return 0;
    }

    virtual int setArgs(const std::vector<std::shared_ptr<args_item>> &arglist);

    virtual int readSDO(int index, int sub_index, int type, int size, void *data);
    virtual int setRunning();

protected:
    int can_id;
    QString name;
    int index;

private:
    static QMutex m_mutex;
    static bool sdo_success;

    //指向信号量的指针
    static sem_t *semW;
    static sem_t *semR;
};
