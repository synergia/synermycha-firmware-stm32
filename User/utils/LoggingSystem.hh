#pragma once

#include "HalUtils.hh"
#include "Logger.hh"

namespace utils
{

class LoggingSystem
{
  public:
    LoggingSystem(LoggingWorkerPtrType worker, volatile bool& isFreeForNextTx);

    Logger info;
    Logger error;
};

}  // namespace utils