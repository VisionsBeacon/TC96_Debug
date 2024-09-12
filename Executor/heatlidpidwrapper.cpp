#include "heatlidpidwrapper.h"

HeatLidPidWrapper::HeatLidPidWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "heatlid_pid")
{
    this->index = 0x260A;
    this->control = 0x01;
    this->status = 0x02;
    this->heatlid_kp = 0x03;
    this->heatlid_ki = 0x04;
    this->heatlid_kd = 0x05;
}

int HeatLidPidWrapper::write_data(int paramName, int32_t value)
{
    int ret = 0;
    switch (paramName)
    {
    case 0:
        ret = write_param(this->heatlid_kp, value);
        break;
    case 1:
        ret = write_param(this->heatlid_ki, value);
        break;
    case 2:
        ret = write_param(this->heatlid_kd, value);
        break;
    }

    return ret;
}

int HeatLidPidWrapper::read_data(int paramName, int32_t &value)
{
    int ret = 0;
    switch (paramName)
    {
    case 0:
        ret = read_param(this->heatlid_kp, value);
        break;
    case 1:
        ret = read_param(this->heatlid_ki, value);
        break;
    case 2:
        ret = read_param(this->heatlid_kd, value);
        break;
    }

    return ret;
}
