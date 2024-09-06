#pragma once

#include <QObject>
#include <memory>

#include "executorwrapper.h"


// 创建一个单例工厂
class ExecutorFactory : public QObject
{
    Q_OBJECT
public:
    static ExecutorFactory& Instance()
    {
        static ExecutorFactory instance;
        return instance;
    }

  //创建一个ExecutorWrapper
  std::shared_ptr<ExecutorWrapper> CreateExecutorWrapper(const QString& name, const QString& type);

private:
    ExecutorFactory() = default;
    ~ExecutorFactory() = default;
    ExecutorFactory(const ExecutorFactory&) = delete;
    ExecutorFactory& operator=(const ExecutorFactory&) = delete;

protected:

};
