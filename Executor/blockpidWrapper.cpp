#include "blockpidWrapper.h"
#include"acturator.h"

BlockPidWrapper::BlockPidWrapper(int can_id, const QString &name):ExecutorWrapper(can_id, name, "block_pid")
{
    this->index=0x2609;
    this->control=0x01;
    this->status=0x02;
    this->block_kp=0x03;
    this->block_ki=0x04;
    this->block_kd=0x05;
}

int BlockPidWrapper::write_data(int paramName, int32_t value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=write_param(this->block_kp,value);
        break;
    case 1:
        ret=write_param(this->block_ki,value);
        break;
    case 2:
        ret=write_param(this->block_kd,value);
        break;

    }
    return ret;

}

int BlockPidWrapper::read_data(int paramName,int32_t &value){
    int ret=0;
    switch (paramName) {
    case 0:
        ret=read_param(this->block_kp,value);
        break;
    case 1:
        ret=read_param(this->block_ki,value);
        break;
    case 2:
        ret=read_param(this->block_kd,value);
        break;

    }
    return ret;
}



