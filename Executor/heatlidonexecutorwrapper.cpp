#include "heatlidonexecutorwrapper.h"

HeatLidOnExecutorWrapper::HeatLidOnExecutorWrapper(int can_id, const QString &name): ExecutorWrapper(can_id, name, "heatlid_on")
{
    this->index=0x2606;
    this->control=0x01;
    this->status=0x02;
    this->temperature=0x03;
}

int HeatLidOnExecutorWrapper::write_data(int paramName,int32_t value){
    int ret=write_param(this->temperature,value);
    return ret;
}

int HeatLidOnExecutorWrapper::read_data(int paramName,int32_t &value){
    int ret=read_param(this->temperature,value);
    return ret;
}
