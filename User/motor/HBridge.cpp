#include "HBridge.hh"

HBridge::HBridge(GpioPort portEnable, GpioPin pinEnable)
    : mPortEnable(portEnable)
    , mPinEnable(pinEnable)
{
}

void HBridge::initialize()
{
    enable();
}

void HBridge::enable()
{
    gpioWrite(mPortEnable, mPinEnable, true);
}

void HBridge::disable()
{
    gpioWrite(mPortEnable, mPinEnable, false);
}