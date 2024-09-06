#ifndef BLOCKPID_H
#define BLOCKPID_H

#include"executorwrapper.h"
class BlockPidWrapper:public ExecutorWrapper
{
public:
    enum ParamName{
        BLOCK_KP=0,
        BLOCK_KI,
        BLOCK_KD,
    };
    BlockPidWrapper(int can_id, const QString &name);
    int write_data(int paramName,int32_t value) override;
    int read_data(int paramName,int32_t &value) override;
    
protected:
    int block_kp;
    int block_ki;
    int block_kd;
    
    
};

#endif // BLOCKPID_H
