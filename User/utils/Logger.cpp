#include "Logger.hh"
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace
{
char stringToPrint[utils::MAX_LEN_LOG];
}

namespace utils
{

Logger::Logger(LoggingWorkerPtrType worker, const char* prefix)
    : mWorker{worker}
{
    mPrefixLen = strlen(prefix);
    if (mPrefixLen > MAX_LEN_PREFIX)
        mPrefixLen = MAX_LEN_PREFIX;

    strncpy(mPrefix, prefix, mPrefixLen);
}

void Logger::operator()(const char* str, ...)
{
    va_list argp;
    va_start(argp, str);

    strncpy(stringToPrint, mPrefix, mPrefixLen);
    char* ptrToWrite = stringToPrint + mPrefixLen;
    int ret          = vsprintf(ptrToWrite, str, argp);
    if (ret < 0)
        return;

    if (ret > MAX_LEN_LOG)
        ret = MAX_LEN_LOG;

    bool logOk = mWorker(stringToPrint, ret);

    va_end(argp);

    if (not logOk)
    {
        // error
    }
}

}  // namespace utils