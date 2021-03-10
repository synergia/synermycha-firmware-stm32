#pragma once

#include "sensors/DistanceTof.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

class Mycha : public utils::Observer
{
  public:
    Mycha(utils::AllSignals& signals);

  private:
    void onInterruptDistance();
    void onInterruptControll();

    utils::AllSignals& mSignals;

    DistanceTof mSensorL;
    DistanceTof mSensorFL;
    DistanceTof mSensorF;
    DistanceTof mSensorFR;
    DistanceTof mSensorR;
};