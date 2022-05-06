#include "LoggingSystem.hh"
#include <cstdarg>
#include <cstdio>

namespace
{
char stringToPrint[100];
}

namespace utils
{

LoggingSystem::LoggingSystem(LoggingWorkerPtrType worker)
    : info{worker, "INF:"}
    , error{worker, "ERR:"}
{
}

}  // namespace utils