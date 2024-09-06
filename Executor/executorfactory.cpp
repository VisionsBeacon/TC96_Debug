#include "executorfactory.h"
#include "lidCloseexecutorwrapper.h"
#include "lidopenexecutorwrapper.h"
#include "rdmlexecutorwrapper.h"
#include "resetexecutorwrapper.h"
#include "startexecutorwrapper.h"
#include "stopexecutorwrapper.h"
#include "heatlidoffwrapper.h"
#include "heatlidonexecutorwrapper.h"
#include "temperatures.h"
#include "blockpidWrapper.h"
#include "heatlidpidwrapper.h"
#include "basicparamwrapper.h"
#include "canparamwrapper.h"
#include "calibratecompensationwrapper.h"
#include "cyclingcompensationwrapper.h"
#include "../Config/datahandler.h"

std::shared_ptr<ExecutorWrapper> ExecutorFactory::CreateExecutorWrapper(const QString& name, const QString& type)
{
    //又把名称转换为can_id
    int can_id = DataHandler::getInstance()->getCanIdForDevice(name);

    if (can_id < 0)
    {
        return nullptr;
    }

    if (type == "reset")
    {
        return std::make_shared<ResetExecutorWrapper>(can_id, name);
    }
    else if (type == "start")
    {
        return std::make_shared<StartExecutorWrapper>(can_id, name);
    }
    else if (type == "stop")
    {
        return std::make_shared<StopExecutorWrapper>(can_id, name);
    }
    else if (type == "set_schema")
    {
        return std::make_shared<RdmlExecutorWrapper>(can_id, name);
    }
    else if (type == "lid_open")
    {
        return std::make_shared<LidOpenExecutorWrapper>(can_id, name);
    }
    else if (type == "heatlidon")
    {
        return std::make_shared<HeatLidOnExecutorWrapper>(can_id, name);
    }
    else if (type == "heatlidoff")
    {
        return std::make_shared<HeatLidOffWrapper>(can_id, name);
    }
    else if (type == "lid_close")
    {
        return std::make_shared<LidCloseExecutorWrapper>(can_id, name);
    }
    else if(type == "temperature")
    {
        return std::make_shared<Temperatures>(can_id, name);
    }
    else if(type == "heatlidpid")
    {
        return std::make_shared<HeatLidPidWrapper>(can_id, name);
    }
    else if(type == "blockpid")
    {
        return std::make_shared<BlockPidWrapper>(can_id, name);
    }
    else if(type == "basicparam")
    {
        return std::make_shared<BasicParamWrapper>(can_id, name);
    }
    else if(type == "canparam")
    {
        return std::make_shared<CanParamWrapper>(can_id, name);
    }
    else if(type == "calibratecompensation")
    {
        return std::make_shared<CalibrateCompensationWrapper>(can_id, name);
    }
    else if(type == "cyclingcompensation")
    {
        return std::make_shared<CyclingCompensationWrapper>(can_id, name);
    }

    return nullptr;
}
