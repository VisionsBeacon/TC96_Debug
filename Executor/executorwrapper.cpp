#include "executorwrapper.h"

#include <errno.h>
#include <list>
#include "acturator.h"
#include "canfestival.h"

#include "waiting_list.h"

static bool is_running(int can_id, int index, int sub_index)
{
    return find_item2(can_id, index, sub_index) != nullptr;
}




bool ExecutorWrapper::in_process()
{
    return is_running(this->can_id, this->index, this->status);
}

void ExecutorWrapper::set_processing()
{
    wait_item item = {can_id, this->index, this->status, &(this->sem)};
    insert_wait_item(item);
}

int ExecutorWrapper::wait_result()
{
    set_processing();
    sem_wait(&(this->sem));
    return set_finished();
}

int ExecutorWrapper::set_finished()
{
    wait_item item = {this->can_id, this->index, this->status, &(this->sem)};
    remove_wait_item(item);
    // qDebug() << "Result wait item " << item.result << endl;
    return item.result;
}

//下发can指令
int ExecutorWrapper::run(const QString &args)
{
    // 判断指令是否已经在运行
    if (this->in_process())
    {
        // SignalManager::instance()->AddMessage("上一个指令正在运行");
        // qDebug() << "Already in process" << endl;
        return -EBUSY;
    }

    Actuator acturator(can_id, this->name);

    //设为运行
    this->set_processing();
    // qDebug() << "Insert Finish" << endl;

    uint32_t value = 1;
    if(acturator.writeSDO(this->index, this->control, uint32, &value, sizeof(uint32_t)) != 0)
    {
        this->set_finished();
        return -EFAULT;
    }

    sem_wait(&(this->sem));
    return set_finished();
}

int ExecutorWrapper::write_param(int param, int32_t value)
{
    int ret = 0;
    if (this->in_process())
    {
        // qDebug() << "Already in process" << endl;
        return -EBUSY;
    }

    Actuator acturator(can_id, this->name, this->index);

    this->set_processing();
    // qDebug() << "Insert Finish" << endl;
    // qDebug() << this->index << endl << param << value;
    if (acturator.writeSDO(this->index, param, uint32, &value, sizeof(value)) != 0)
    {
        this->set_finished();
        return -EFAULT;
    }

    ret = acturator.setRunning();
    // qDebug() << " setRunning ret" << ret;
    if(ret == 0)
    {
        sem_wait(&(this->sem));
        return set_finished();
    }

    return set_finished();
}

//读取参数，通过引用返回参数
int ExecutorWrapper::read_param(int param, int32_t &value)
{
    Actuator acturator(can_id, this->name);
    int ret = acturator.readSDO(this->index, param, uint32, sizeof(value), &value);
    return ret;
}
