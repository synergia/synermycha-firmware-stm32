#pragma once

#include "HalUtils.hh"

namespace utils
{
// better to have bigger buffer to not overwrite memory
constexpr int SAFE_BUF_LEN   = 300;
constexpr int MAX_LEN_LOG    = 250;
constexpr int MAX_LEN_PREFIX = 4;

class Logger
{
  public:
    // prefix can be at most 3 chars long
    Logger(LoggingWorkerPtrType worker, const char* prefix, volatile bool& isFreeForNextTx);

    void operator()(const char* str, ...);

  private:
    // function to display, save or something else string to log
    LoggingWorkerPtrType mWorker;
    int mPrefixLen;
    char mPrefix[MAX_LEN_PREFIX];

    volatile bool& mIsFreeForNextTx;
};

}  // namespace utils