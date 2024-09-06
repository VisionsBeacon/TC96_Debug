#include "basicparamwrapper.h"

BasicParamWrapper::BasicParamWrapper(int can_id, const QString &name):ExecutorWrapper(can_id, name, "basic_param")
{
    this->index=0x260B;
    this->control=0x01;
    this->status=0x02;
    this->lid_move_distance=0x03;
}

int BasicParamWrapper::write_data(int paramName,int32_t value){
    int ret=write_param(this->lid_move_distance,value);
    return ret;
}

int BasicParamWrapper::read_data(int paramName,int32_t &value){
    int ret=read_param(this->lid_move_distance,value);
    return ret;
}
