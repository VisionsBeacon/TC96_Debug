#include "startexecutorwrapper.h"

StartExecutorWrapper::StartExecutorWrapper(int can_id, const QString &) : ExecutorWrapper(can_id,name,"start")
{
    this->index=0x2602;
    this->control=0x01;
    this->status=0x02;
}
