#include "LoggingSystem.hh"
#include <cstdarg>
#include <cstdio>

namespace utils
{

LoggingSystem::LoggingSystem(LoggingWorkerPtrType worker, volatile bool& isFreeForNextTx)
    : info{worker, "INF:", isFreeForNextTx}
    , error{worker, "ERR:", isFreeForNextTx}
{
}

}  // namespace utils