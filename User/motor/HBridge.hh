#pragma once

#include "utils/HalUtils.hh"

class HBridge
{
  public:
    HBridge(GpioPort portEnable, GpioPin pinEnable);
    void initialize();

    void enable();
    void disable();

  private:
    GpioPort mPortEnable;
    GpioPin mPinEnable;
};