#pragma once

#include "HalUtils.hh"

namespace utils
{
constexpr int MAX_LEN_LOG    = 100;
constexpr int MAX_LEN_PREFIX = 4;

class Logger
{
  public:
    // prefix can be at most 3 chars long
    Logger(LoggingWorkerPtrType worker, const char* prefix);

    void operator()(const char* str, ...);

  private:
    // function to display, save or something else string to log
    LoggingWorkerPtrType mWorker;
    int mPrefixLen;
    char mPrefix[3];
};

}  // namespace utils