#ifndef CANPARAM_H
#define CANPARAM_H

#include"executorwrapper.h"
class CanParamWrapper:public ExecutorWrapper
{
public:
    CanParamWrapper(int can_id, const QString &name);
    int write_data(int paramName, int32_t value);
    int read_data(int paramName, int32_t &value);

protected:
    int can_id;
    int baundrate;
};

#endif // CANPARAM_H
