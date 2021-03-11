#include "HBridge.hh"

HBridge::HBridge(GpioPort portEnable, GpioPin pinEnable)
    : mPortEnable(portEnable)
    , mPinEnable(pinEnable)
{
}

void HBridge::enable()
{
    GpioWrite(mPortEnable, mPinEnable, true);
}

void HBridge::disable()
{
    GpioWrite(mPortEnable, mPinEnable, false);
}