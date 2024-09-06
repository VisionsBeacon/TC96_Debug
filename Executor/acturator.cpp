#include "acturator.h"
#include "../eds/eds_master.h"

//用于读写的两个信号量
static sem_t sem_memW;
static sem_t sem_memR;

QMutex Actuator::m_mutex;
bool Actuator::sdo_success = false;
sem_t* Actuator::semW = nullptr;        //指向信号量的指针
sem_t* Actuator::semR = nullptr;

static void call_backW(CO_Data* d, UNS8 nodeid)
{
    //释放信号量
    sem_post(&sem_memW);
}

static void call_backR(CO_Data* d, UNS8 nodeid)
{
    //释放信号量
    sem_post(&sem_memR);
}

static int write_sdo(int can_id, int index, int subindex, int type, void* value, int size)
{
    qDebug() << "Start To Write SDO";

    EnterMutex();
    if (writeNetworkDictCallBack(&eds_master_Data, can_id, index, subindex, size, type, value, call_backW, 0) == 0)
    {
        LeaveMutex();

        //进入阻塞
        sem_wait(&sem_memW);
    }
    else
    {
        LeaveMutex();
        qDebug() << "Write SDO Failed";
    }

    UNS32 abortCode;
    UNS8 ret = getWriteResultNetworkDict(&eds_master_Data, can_id, &abortCode);
    closeSDOtransfer(&eds_master_Data, can_id, SDO_CLIENT);

    qDebug() << "LeaveMutex";

    if (ret != SDO_FINISHED)
    {
        qDebug() << "Write SDO Failed " << std::hex << index << "," << subindex;
        return -EFAULT;
    }
    return ret == SDO_FINISHED ? 0 : -1;
}

static int read_sdo(int can_id, int index, int sub_index, int type, int size, void* data)
{
    qDebug() << "Start To Read SDO";
    EnterMutex();

    if(readNetworkDictCallback(&eds_master_Data, can_id, index, sub_index, type, call_backR, 0) == 0)
    {
        LeaveMutex();

        //进入阻塞
        sem_wait(&sem_memR);
    }
    else
    {
        LeaveMutex();
        qDebug() << "Read SDO Failed";
    }

    UNS32 abortCode;
    UNS32 read_size = size;
    UNS8 ret = getReadResultNetworkDict(&eds_master_Data, can_id, data, &read_size, &abortCode);

    qDebug() << "LeaveMutex";

    if (ret != SDO_FINISHED)
    {
        qDebug() << "Read SDO Failed " << std::hex << index << "," << sub_index;
        return -EFAULT;
    }

    return ret == SDO_FINISHED ? 0 : -1;
}

// 设置running
int Actuator::setRunning()
{
    int32_t value = 1;
    qDebug() << "setRunning" << this->index << "value: " << value;
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

int Actuator::writeSDO(int index, int subindex, int type, void* value, int size)
{
    QMutexLocker lock(&m_mutex);

    if (Actuator::semW == nullptr)
    {
        Actuator::semW = &sem_memW;

        /*  初始化 semW 信号量，并将其初始值设为 0，
            这意味着此信号量一开始处于阻塞状态。
            任何试图调用 sem_wait(semW) 的线程都会被阻塞，
            直到另一个线程通过 sem_post(semW) 增加该信号量的值，唤醒等待的线程。
        */
        sem_init(Actuator::semW, 0, 0);
    }

    return write_sdo(this->can_id, index, subindex, type, value, size);
}

int Actuator::readSDO(int index, int sub_index, int type, int size, void* data)
{
    QMutexLocker lock(&m_mutex);

    //初始化信号量指针
    if(Actuator::semR == nullptr)
    {
        Actuator::semR = &sem_memR;

        /*  初始化 semR 信号量，并将其初始值设为 0，
            这意味着此信号量一开始处于阻塞状态。
            任何试图调用 sem_wait(semR) 的线程都会被阻塞，
            直到另一个线程通过 sem_post(semR) 增加该信号量的值，唤醒等待的线程。
        */
        sem_init(Actuator::semR, 0, 0);
    }

    return read_sdo(this->can_id, index, sub_index, type, size, data);
}
