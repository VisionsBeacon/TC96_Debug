#include "acturator.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "eds_master.h"

#ifdef __cplusplus
};
#endif



QMutex Actuator::m_mutex;

static QSemaphore sem_memW(0);  // QSemaphore 初始化，值为 0
static QSemaphore sem_memR(0);

QSemaphore* Actuator::semW = nullptr;
QSemaphore* Actuator::semR = nullptr;


bool Actuator::sdo_success = false;

static void call_backW(CO_Data* d, UNS8 nodeid)
{
    sem_memW.release();
}
static void call_backR(CO_Data* d, UNS8 nodeid)
{
    sem_memR.release();
}

static int write_sdo(int can_id, int index, int subindex, int type, void* value, int size)
{
    // qDebug() << "Start To Write SDO" << endl;

    EnterMutex();
    if (writeNetworkDictCallBack(&eds_master_Data, can_id, index, subindex, size, type, value, call_backW, 0) == 0)
    {
        LeaveMutex();

        sem_memW.acquire();
    }
    else
    {
        LeaveMutex();
        // qDebug() << "Write SDO Failed" << endl;
    }

    UNS32 abortCode;
    UNS8 ret = getWriteResultNetworkDict(&eds_master_Data, can_id, &abortCode);
    closeSDOtransfer(&eds_master_Data, can_id, SDO_CLIENT);

    // qDebug() << "LeaveMutex" << endl;

    if (ret != SDO_FINISHED)
    {
        // qDebug() << "Write SDO Failed " << std::hex << index << "," << subindex << endl;
        return -EFAULT;
    }
    return ret == SDO_FINISHED ? 0 : -1;
}

static int read_sdo(int can_id, int index, int sub_index, int type, int size, void* data)
{
    // qDebug() << "Start To Read SDO" << endl;
    EnterMutex();

    if(readNetworkDictCallback(&eds_master_Data, can_id, index, sub_index, type, call_backR, 0) == 0)
    {
        LeaveMutex();
        sem_memR.acquire();
    }
    else
    {
        LeaveMutex();
        // qDebug() << "Read SDO Failed" << endl;
    }

    UNS32 abortCode;
    UNS32 read_size = size;
    UNS8 ret = getReadResultNetworkDict(&eds_master_Data, can_id, data, &read_size, &abortCode);

    // qDebug() << "LeaveMutex" << endl;

    if (ret != SDO_FINISHED)
    {
        // qDebug() << "Read SDO Failed " << std::hex << index << "," << sub_index << endl;
        return -EFAULT;
    }

    return ret == SDO_FINISHED ? 0 : -1;
}

int Actuator::writeSDO(int index, int subindex, int type, void* value, int size)
{
    QMutexLocker lock(&m_mutex);

    if (Actuator::semW == nullptr)
    {
        Actuator::semW = &sem_memW;
    }

    return write_sdo(this->can_id, index, subindex, type, value, size);
}

// 设置running
int Actuator::setRunning()
{
    int32_t value = 1;
    // qDebug() << "setRunning" << this->index << endl << value << value;
    return this->writeSDO(this->index, 0x01, uint32, &value, sizeof(value));
}

int Actuator::setArgs(const std::vector<std::shared_ptr<args_item>>& args_list)
{
    int ret = 0;
    for(auto& item : args_list)
    {
        if(this->writeSDO(item->index, item->sub_index, item->get_data_type(), item->data(), item->size()) != 0)
        {
            ret = -EFAULT;
            break;
        }
    }

    return ret;
}

int Actuator::readSDO(int index, int sub_index, int type, int size, void* data)
{
    QMutexLocker lock(&m_mutex);

    if (Actuator::semR == nullptr)
    {
        Actuator::semR = &sem_memR;
    }

    return read_sdo(this->can_id, index, sub_index, type, size, data);
}


