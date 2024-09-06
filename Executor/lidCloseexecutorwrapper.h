#ifndef LIDONEXECUTORWRAPPER_H
#define LIDONEXECUTORWRAPPER_H

#include"executorwrapper.h"
class LidCloseExecutorWrapper : public ExecutorWrapper
{
public:
    LidCloseExecutorWrapper(int can_id, const QString &name);
};

#endif // LIDONEXECUTORWRAPPER_H
