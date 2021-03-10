#pragma once

#include "Led.hh"
#include "sensors/DistanceTof.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

class Mycha : public utils::Observer
{
  public:
    Mycha(utils::AllSignals& signals);

  private:
    void onInterruptDistance();
    void onInterruptController();

    void buttonUp();
    void buttonDown();
    void tim14Elapsed();

    utils::AllSignals& mSignals;

    DistanceTof mSensorL;
    DistanceTof mSensorFL;
    DistanceTof mSensorF;
    DistanceTof mSensorFR;
    DistanceTof mSensorR;

    Led mLed1;
    Led mLed2;
    Led mLed3;
    Led mLed4;
    Led mLed5;
};