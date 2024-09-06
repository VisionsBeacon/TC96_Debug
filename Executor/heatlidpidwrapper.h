#ifndef HEATLIDPID_H
#define HEATLIDPID_H

#include"executorwrapper.h"
class HeatLidPidWrapper : public ExecutorWrapper
{
public:
    HeatLidPidWrapper(int can_id, const QString &name);
    int write_data(int paramName, int32_t value);
    int read_data(int paramName, int32_t &value);
protected:
    int heatlid_kp;
    int heatlid_ki;
    int heatlid_kd;
    
};

#endif // HEATLIDPID_H
