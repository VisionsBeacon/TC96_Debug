#include "lidopenexecutorwrapper.h"

LidOpenExecutorWrapper::LidOpenExecutorWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "lid_open")
{
    this->index = 0x2604;
    this->control = 0x01;
    this->status = 0x02;
}

