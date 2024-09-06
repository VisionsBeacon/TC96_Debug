#include <unistd.h>

#include "acturator.h"
#include "acturator_args.h"
#include "rdmlexecutorwrapper.h"
#include "waiting_list.h"


RdmlExecutorWrapper::RdmlExecutorWrapper(int can_id, const QString &name) : ExecutorWrapper(can_id, name, "rdml")
{
    this->index=0x2608;
    this->control=0x01;
    this->status=0x02;
}

int RdmlExecutorWrapper::write_rdml_size(int size)
{
  Actuator acturator(this->can_id, this->name, 0x2608);
  return acturator.writeSDO(0x2608, 0x03, uint32, &size, 4);
}

int RdmlExecutorWrapper::write_rdml_data(const char *rdml, int size)
{
  uint8_t buf[32];
  int written = 0;
  int ret = 0;
  std::vector<std::shared_ptr<args_item>> args_list;

  Actuator acturator(this->can_id, this->name, 0x2608);

  while (written < size)
  {
    args_list.clear();

    int to_write = size - written > 32 ? 32 : size - written;
    memcpy(buf, rdml + written, to_write);

    args_list.push_back(std::make_shared<args_int>("offset", 0x2608, 0x04, written));

    args_list.push_back(std::make_shared<args_array>("data", 0x2608, 0x05, buf));

    ret = acturator.setArgs(args_list);

    if (ret == 0)
    {
      ret = acturator.setRunning();
    }
    else
    {

    }

    if (ret == 0)
    {
      sem_wait(&sem);
    }

    if (ret != 0)
    {
      break;
    }

    written += sizeof(buf);
  }

  return ret;
}

int RdmlExecutorWrapper::run(const QString &args)
{
    if (this->in_process())
    {
        return -EBUSY;
    }

    // //string转json
    // Json::Reader reader;
    // Json::Value root;
    // if(reader.parse(args.c_str(), root));

    // // Start Run
    // qDebug() << "Start Run" << endl;

    // this->set_processing();

    // int ret = 0;

    // const char *rdml = root["rdml"].asCString();
    // int size = strlen(rdml);

    // ret = this->write_rdml_size(size);
    // if (ret == 0) {
    //     ret = this->write_rdml_data(rdml, size);
    // }

    // if (ret == 0) {
    //     ret = this->set_finished();
    // } else {
    //     this->set_finished();
    // }

    // return ret;
}
