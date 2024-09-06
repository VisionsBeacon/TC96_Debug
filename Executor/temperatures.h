#ifndef TEMPERATURES1_H
#define TEMPERATURES1_H

#include "executorwrapper.h"
class Temperatures:public ExecutorWrapper
{
public:
    Temperatures(int can_id, const QString &name);
    int read_data(int paramName, int32_t &value) ;

private:
    int temperatures1;
    int temperatures2;
    int temperatures3;
    int temperatures4;
    int temperatures5;

};

#endif // TEMPERATURES1_H
