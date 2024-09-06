#include "temperatures.h"

Temperatures::Temperatures(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "temperature")
{
    this->index = 0x260F;
    this->temperatures1 = 0x01;
    this->temperatures2 = 0x02;
    this->temperatures3 = 0x03;
    this->temperatures4 = 0x04;
    this->temperatures5 = 0x05;
}
int Temperatures::read_data(int paramName, int32_t &value)
{
    int ret = 0;
    switch (paramName)
    {
    case 0:
        ret = read_param(this->temperatures1, value);
        break;
    case 1:
        ret = read_param(this->temperatures2, value);
        break;
    case 2:
        ret = read_param(this->temperatures3, value);
        break;
    case 3:
        ret = read_param(this->temperatures4, value);
        break;
    case 4:
        ret = read_param(this->temperatures5, value);
        break;
    }

    return ret;
}
