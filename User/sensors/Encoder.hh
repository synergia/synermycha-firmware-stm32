#pragma once

#include "utils/HalUtils.hh"

namespace sensors
{

class Encoder
{
  public:
    Encoder(TimHandler* handler);
    void initialize();

    int32_t getDiffTicks();

  private:
    TimHandler* mHandler;
    uint32_t mLastCnt;
    bool isInitialized;
};

}  // namespace sensors