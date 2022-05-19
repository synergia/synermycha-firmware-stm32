#include "LoggingSystem.hh"
#include <cstdarg>
#include <cstdio>

namespace utils
{

LoggingSystem::LoggingSystem(LoggingWorkerPtrType worker)
    : info{worker, "INF:"}
    , error{worker, "ERR:"}
{
}

}  // namespace utils