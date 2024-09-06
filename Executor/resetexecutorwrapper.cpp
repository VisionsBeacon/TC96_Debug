#include "resetexecutorwrapper.h"

ResetExecutorWrapper::ResetExecutorWrapper(int can_id, const QString &name):ExecutorWrapper(can_id,name,"reset")
{
    this->index=0x2601;
    this->control=0x01;
    this->status=0x02;
}
