#include "stopexecutorwrapper.h"

StopExecutorWrapper::StopExecutorWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id,name,"stop")
{

    this->index=0x2603;
    this->control=0x01;
    this->status=0x02;
}
