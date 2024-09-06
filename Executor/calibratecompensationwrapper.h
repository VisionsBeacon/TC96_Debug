#ifndef CALIBRATECOMPENSATION_H
#define CALIBRATECOMPENSATION_H

#include"executorwrapper.h"
class CalibrateCompensationWrapper : public ExecutorWrapper
{
public:
    CalibrateCompensationWrapper(int can_id, const QString &name);
    int write_data(int paramName,int32_t value) override;
    int read_data(int paramName,int32_t &value) override;
    
    
protected:
    int block_1;
    int block_2;
    int block_3;
    int heat_lid;
    int radiator;


};

#endif // CALIBRATECOMPENSATION_H
