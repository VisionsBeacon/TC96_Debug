#ifndef STARTEXECUTORWRAPPER_H
#define STARTEXECUTORWRAPPER_H

#include "executorwrapper.h"
class StartExecutorWrapper:public ExecutorWrapper
{
public:
    StartExecutorWrapper(int can_id, const QString &name);
};

#endif // STARTEXECUTORWRAPPER_H
