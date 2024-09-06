#include "cyclingcompensationwrapper.h"

CyclingCompensationWrapper::CyclingCompensationWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "cycle_compensation")
{
    this->index=0x260E;
    this->control=0x01;
    this->status=0x02;
    this->cycling_k=0x03;
    this->cycling_b=0x04;
}
int CyclingCompensationWrapper::write_data(int paramName, int32_t value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=write_param(this->cycling_k,value);
        break;
    case 1:
        ret=write_param(this->cycling_b,value);
        break;
    }
    return ret;
}

int CyclingCompensationWrapper::read_data(int paramName, int32_t &value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=read_param(this->cycling_k,value);
        break;
    case 1:
        ret=read_param(this->cycling_b,value);
        break;
    }
    return ret;
}
