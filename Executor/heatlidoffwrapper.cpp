#include "heatlidoffwrapper.h"

HeatLidOffWrapper::HeatLidOffWrapper(int can_id, const QString &name):ExecutorWrapper(can_id ,name, "heatlid_off")
{
    this->index=0x2607;
    this->control=0x01;
    this->status=0x02;
}

