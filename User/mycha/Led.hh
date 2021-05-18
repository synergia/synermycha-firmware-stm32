#pragma once

#include "utils/AllSignals.hh"
#include "utils/HalUtils.hh"
#include "utils/Observer.hh"

namespace mycha
{

class Led : public utils::Observer
{
  public:
    Led(utils::AllSignals& sig, GpioPort port, GpioPin pin, bool isOn = false);
    void initialize();

    void on();
    void off();
    bool isOn();
    void toggle();
    void set(bool isOn);

  private:
    utils::AllSignals& mAllSignals;
    GpioPort mPort;
    GpioPin mPin;

    bool mIsOn;
};

}  // namespace mycha