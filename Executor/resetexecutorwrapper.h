#ifndef RESETEXECUTORWRAPPER_H
#define RESETEXECUTORWRAPPER_H

#include "executorwrapper.h"
class ResetExecutorWrapper : public ExecutorWrapper
{
public:
    ResetExecutorWrapper(int can_id, const QString &name);
};

#endif // RESETEXECUTORWRAPPER_H
