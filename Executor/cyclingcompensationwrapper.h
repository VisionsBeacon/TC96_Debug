#ifndef CYCLINGCOMPENSATION_H
#define CYCLINGCOMPENSATION_H

#include"executorwrapper.h"
class CyclingCompensationWrapper:public ExecutorWrapper
{
public:
    enum ParamName{
        CYCLING_K=0,
        CYCLING_B,

    };
    CyclingCompensationWrapper(int can_id, const QString &name);
    int write_data(int paramName,int32_t value);
    int read_data(int paramName,int32_t &value);
protected:
    int cycling_k;
    int cycling_b;
};

#endif // CYCLINGCOMPENSATION_H
