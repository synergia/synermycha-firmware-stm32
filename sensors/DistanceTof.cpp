#include "DistanceTof.hh"

DistanceTof::DistanceTof(GpioPort port, GpioPin pin, TofAddress address)
    : mPort(port)
    , mPin(pin)
    , mAddress(address)
    , mLastMeasurement(0)
{
    memset(&mHandler, 0, sizeof(mHandler));

    GpioWrite(mPort, mPin, false);
    WaitMs(20);

    setup_VL53L0X(&mHandler);
    GpioWrite(mPort, mPin, true);
    HAL_Delay(10);
    init(&mHandler, true);
    setAddress(&mHandler, mAddress);
    setTimeout(&mHandler, 35);
    startContinuous(&mHandler, 33);
}

uint16_t DistanceTof::getLastMeasurement() const
{
    return mLastMeasurement;
}

uint16_t DistanceTof::readDistance()
{
    mLastMeasurement = readRangeContinuousMillimeters(&mHandler);
    if (timeoutOccurred(&mHandler))
        startContinuous(&mHandler, 33);

    return mLastMeasurement;
}