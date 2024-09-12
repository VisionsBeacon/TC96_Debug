#include "canparamwrapper.h"

CanParamWrapper::CanParamWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "can_param")
{
    this->index = 0x260C;
    this->control = 0x01;
    this->status = 0x02;
    this->can_id = 0x03;
    this->baundrate = 0x04;
}

int CanParamWrapper::write_data(int paramName, int32_t value)
{
    int ret = 0;
    switch (paramName)
    {
    case 0:
        ret = write_param(this->can_id, value);
        break;
    case 1:
        ret = write_param(this->baundrate, value);
        break;
    }

    return ret;
}

int CanParamWrapper::read_data(int paramName, int32_t &value)
{
    int ret = 0;
    switch (paramName)
    {
    case 0:
        ret=read_param(this->can_id, value);
        break;
    case 1:
        ret=read_param(this->baundrate, value);
        break;
    }

    return ret;
}
