#include "calibratecompensationwrapper.h"

CalibrateCompensationWrapper::CalibrateCompensationWrapper(int can_id, const QString &name):ExecutorWrapper(can_id, name, "calibrate_compensation")
{
    this->index=0x260D;
    this->control=0x01;
    this->status=0x02;
    this->block_1=0x03;
    this->block_2=0x04;
    this->block_3=0x05;
    this->heat_lid=0x06;
    this->radiator=0x07;
}

int CalibrateCompensationWrapper::write_data(int paramName, int32_t value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=write_param(this->block_1,value);
        break;
    case 1:
        ret=write_param(this->block_2,value);
        break;
    case 2:
        ret=write_param(this->block_3,value);
        break;
    case 3:
        ret=write_param(this->heat_lid,value);
        break;
    case 4:
        ret=write_param(this->radiator,value);
        break;

    }
    return ret;
}

int CalibrateCompensationWrapper::read_data(int paramName,int32_t &value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=read_param(this->block_1,value);
        break;
    case 1:
        ret=read_param(this->block_2,value);
        break;
    case 2:
        ret=read_param(this->block_3,value);
        break;
    case 3:
        ret=read_param(this->heat_lid,value);
        break;
    case 4:
        ret=read_param(this->radiator,value);
        break;

    }
    return ret;
}
