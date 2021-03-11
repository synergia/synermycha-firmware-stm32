#pragma once

#include "utils/HalUtils.hh"

class Encoder
{
  public:
    Encoder(TimHandler* handler);
    void initialize();

    uint32_t getTicks() const;

  private:
    TimHandler* mHandler;
};