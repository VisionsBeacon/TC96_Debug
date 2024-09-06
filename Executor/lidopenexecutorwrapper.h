#ifndef LIDOPENEXECUTORWRAPPER_H
#define LIDOPENEXECUTORWRAPPER_H

#include"executorwrapper.h"
class LidOpenExecutorWrapper : public ExecutorWrapper
{
public:
    LidOpenExecutorWrapper(int can_id, const QString &name);
};

#endif // LIDOPENEXECUTORWRAPPER_H
