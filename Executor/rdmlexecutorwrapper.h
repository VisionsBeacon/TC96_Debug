#ifndef RDMLEXECUTORWRAPPER_H
#define RDMLEXECUTORWRAPPER_H

#include"executorwrapper.h"
class RdmlExecutorWrapper : public ExecutorWrapper
{
public:
    RdmlExecutorWrapper(int can_id, const QString &name);

    int run(const QString &args) override;

private:
    int write_rdml_size(int size);
    int write_rdml_data(const char* data,int size);

};

#endif // RDMLEXECUTORWRAPPER_H
