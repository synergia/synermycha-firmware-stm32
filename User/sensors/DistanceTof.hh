#pragma once

#include "VL53L0X.h"
#include "utils/HalUtils.hh"

class DistanceTof
{
    using TofAddress = uint8_t;

  public:
    DistanceTof(GpioPort port, GpioPin pin, TofAddress address);
    void initialize();

    uint16_t getLastMeasurement() const;
    uint16_t readDistance();

  private:
    GpioPort mPort;
    GpioPin mPin;
    TofAddress mAddress;

    VL53L0X mHandler;
    uint16_t mLastMeasurement;
};