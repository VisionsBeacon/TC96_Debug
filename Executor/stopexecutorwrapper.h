#ifndef STOPEXECUTORWRAPPER_H
#define STOPEXECUTORWRAPPER_H

#include "executorwrapper.h"

class StopExecutorWrapper:public ExecutorWrapper
{
public:
    StopExecutorWrapper(int can_id, const QString &name);
};

#endif // STOPEXECUTORWRAPPER_H
