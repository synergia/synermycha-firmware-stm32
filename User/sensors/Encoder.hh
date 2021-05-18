#pragma once

#include "utils/HalUtils.hh"

namespace sensors
{

class Encoder
{
  public:
    Encoder(TimHandler* handler);
    void initialize();

    uint32_t getTicks() const;

  private:
    TimHandler* mHandler;
};

}  // namespace sensors