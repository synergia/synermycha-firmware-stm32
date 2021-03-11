#include "Led.hh"

Led::Led(utils::AllSignals& sig, GpioPort port, GpioPin pin, bool isOn)
    : mAllSignals(sig)
    , mPort(port)
    , mPin(pin)
    , mIsOn(isOn)
{
}

void Led::initialize()
{
    on();
    WaitMs(100);
    off();
}

void Led::on()
{
    GpioWrite(mPort, mPin, true);
    mIsOn = true;
}

void Led::off()
{
    GpioWrite(mPort, mPin, false);
    mIsOn = false;
}

bool Led::isOn()
{
    return mIsOn;
}

void Led::toggle()
{
    if (mIsOn)
    {
        off();
        mIsOn = false;
    }
    else
    {
        on();
        mIsOn = true;
    }
}

void Led::set(bool isOn)
{
    if (isOn)
        on();
    else
        off();
}