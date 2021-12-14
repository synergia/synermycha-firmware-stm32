#include "CommandQueue.hh"

namespace controller
{

void CommandQueue::addCommand(const Command& command)
{
    if (not isFull())
    {
        mCommands[mInsertIdx] = command;
        ++mInsertIdx;
    }
}

Command CommandQueue::getNextCommand()
{
    Command ret{};
    if (not isEmpty())
    {
        ret = mCommands[mReadIdx];
        ++mReadIdx;

        if (mReadIdx == mInsertIdx)
        {
            mReadIdx   = 0;
            mInsertIdx = 0;
        }
    }

    return ret;
}

bool CommandQueue::isEmpty()
{
    return mInsertIdx == 0;
}

bool CommandQueue::isFull()
{
    return mInsertIdx == COMMAND_QUEUE_SIZE;
}

}  // namespace controller