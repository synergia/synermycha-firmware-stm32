#pragma once

#include <main.h>

namespace utils
{

class Timer
{
  public:
    Timer() = default;
    void start();
    void stop();
    void reset();
    uint32_t getTimeInUs();
    uint32_t getTimeInMs();

  private:
    uint32_t mLastValueInUs;
};

}  // namespace utils