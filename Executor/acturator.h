// 包含一次
#pragma once

#include <QObject>  //此处如果改为#include <QString>会报错
#include <QMutex>
#include <QSemaphore>
#include <vector>

#include "acturator_args.h"


class Actuator
{

//此处如果添加Q_OBJECT会报错

public:
    Actuator(int can_id, const QString &name = "None", int index = 0x2600)
        : can_id(can_id), name(name), index(index)
    {

    }

    virtual int writeSDO(int index, int subindex, int type, void *value, int size) final;

    virtual int setArgs(std::vector<args_item *> &value)
    {
        for (auto i : value)
        {
            // qDebug() << this->name.c_str() << " setArgs" << i->data() << endl;
        }

        return 0;
    }

    virtual int setArgs(const std::vector<std::shared_ptr<args_item>> &arglist);

    virtual int readSDO(int index, int sub_index, int type, int size, void *data);
    virtual int setRunning();

private:
    static QMutex m_mutex;
    static bool sdo_success;

    //指向信号量的指针
    static QSemaphore* semW;
    static QSemaphore* semR;

protected:
    int can_id;
    QString name;
    int index;
};
