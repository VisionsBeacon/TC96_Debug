#ifndef BASICPARAM_H
#define BASICPARAM_H

#include"executorwrapper.h"
class BasicParamWrapper:public ExecutorWrapper
{

public:
    BasicParamWrapper(int can_id,const QString &name);
    int write_data(int paramName, int32_t value);
    int read_data(int paramName, int32_t &value);
protected:
    int lid_move_distance;

};

#endif // BASICPARAM_H
