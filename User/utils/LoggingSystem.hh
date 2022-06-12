#pragma once

#include "HalUtils.hh"
#include "Logger.hh"

namespace utils
{

class LoggingSystem
{
  public:
    LoggingSystem(LoggingWorkerPtrType worker, volatile bool& isFreeForNextTx);
    LoggingSystem(const LoggingSystem&)  = delete;
    LoggingSystem(const LoggingSystem&&) = delete;

    Logger info;
    Logger error;
};

}  // namespace utils