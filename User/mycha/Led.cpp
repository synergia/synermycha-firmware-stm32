#include "Led.hh"

namespace mycha
{

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
    gpioWrite(mPort, mPin, true);
    mIsOn = true;
}

void Led::off()
{
    gpioWrite(mPort, mPin, false);
    mIsOn = false;
}

bool Led::isOn()
{
    return mIsOn;
}

void Led::toggle()
{
    set(not mIsOn);
}

void Led::set(bool isOn)
{
    isOn ? on() : off();
}

}  // namespace mycha