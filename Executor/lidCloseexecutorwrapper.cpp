#include "lidCloseexecutorwrapper.h"

LidCloseExecutorWrapper::LidCloseExecutorWrapper(int can_id, const QString &name):ExecutorWrapper(can_id, name, "lid_close")
{
    this->index=0x2605;
    this->control=0x01;
    this->status=0x02;
}

