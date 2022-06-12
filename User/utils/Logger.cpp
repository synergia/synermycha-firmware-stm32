#include "Logger.hh"
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace
{
char stringToPrint[utils::SAFE_BUF_LEN];
}  // namespace

namespace utils
{

Logger::Logger(LoggingWorkerPtrType worker, const char* prefix, volatile bool& isFreeForNextTx)
    : mWorker{worker}
    , mIsFreeForNextTx{isFreeForNextTx}
{
    mPrefixLen = strlen(prefix);
    if (mPrefixLen > MAX_LEN_PREFIX)
        mPrefixLen = MAX_LEN_PREFIX;

    strncpy(mPrefix, prefix, mPrefixLen);
}

void Logger::operator()(const char* str, ...)
{
    while (not mIsFreeForNextTx)
        ;
    mIsFreeForNextTx = false;

    uint32_t totalStrLen = 0;
    strncpy(stringToPrint, mPrefix, mPrefixLen);
    totalStrLen      = mPrefixLen;
    char* ptrToWrite = stringToPrint + mPrefixLen;

    va_list argp;
    va_start(argp, str);
    int ret = vsprintf(ptrToWrite, str, argp);
    if (ret < 0)
        return;
    totalStrLen += ret;

    if (totalStrLen > MAX_LEN_LOG)
        totalStrLen = MAX_LEN_LOG;

    // add NL to end of line to help PC logging
    if (stringToPrint[totalStrLen - 1] != '\n')
    {
        stringToPrint[totalStrLen - 1] = '\n';
    }

    bool logOk = mWorker(stringToPrint, totalStrLen);

    va_end(argp);

    if (not logOk)
    {
        // error
    }
}

}  // namespace utils