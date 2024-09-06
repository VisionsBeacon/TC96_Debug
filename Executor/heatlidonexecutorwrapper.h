#ifndef HEATLIDONEXECUTORWRAPPER_H
#define HEATLIDONEXECUTORWRAPPER_H

#include"executorwrapper.h"
class HeatLidOnExecutorWrapper : public ExecutorWrapper
{
public:
    HeatLidOnExecutorWrapper(int can_id, const QString &name);
    int write_data(int paramName, int32_t value);
    int read_data(int paramName, int32_t &value);
protected:
    int temperature;
};

#endif // HEATLIDONEXECUTORWRAPPER_H
