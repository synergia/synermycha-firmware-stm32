#pragma once

#include "HalUtils.hh"
#include "Logger.hh"

namespace utils
{

class LoggingSystem
{
  public:
    LoggingSystem(LoggingWorkerPtrType worker);

    Logger info;
    Logger error;
};

}  // namespace utils