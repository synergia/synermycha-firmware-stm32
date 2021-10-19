#pragma once

#include "utils/HalUtils.hh"

namespace sensors
{

class Encoder
{
  public:
    Encoder(TimHandler* handler);
    void initialize();

    int32_t getDiffTicks() const;

  private:
    TimHandler* mHandler;
};

}  // namespace sensors