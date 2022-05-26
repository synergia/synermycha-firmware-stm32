#pragma once

#include "Command.hh"
#include <array>
#include <cstdint>

namespace controller
{

constexpr uint32_t COMMAND_QUEUE_SIZE{200};

// at this moment, inserting few commands is possible only when queue is empty
// to insert next elems, all commands must be read
class CommandQueue
{
  public:
    CommandQueue() = default;

    void addCommand(const Command& command);
    Command getNextCommand();
    bool isEmpty();
    bool isFull();

  private:
    std::array<Command, COMMAND_QUEUE_SIZE> mCommands;
    uint32_t mInsertIdx{0};
    uint32_t mReadIdx{0};
};

}  // namespace controller