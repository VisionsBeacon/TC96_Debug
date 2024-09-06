#ifndef HEATLIDOFF_H
#define HEATLIDOFF_H

#include"executorwrapper.h"
class HeatLidOffWrapper : public ExecutorWrapper
{
public:
    HeatLidOffWrapper(int can_id, const QString &name);
};

#endif // HEATLIDOFF_H
