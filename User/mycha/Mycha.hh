#pragma once

#include "Led.hh"
#include "motor/HBridge.hh"
#include "motor/Motor.hh"
#include "sensors/DistanceTof.hh"
#include "sensors/Encoder.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

class Mycha : public utils::Observer
{
  public:
    Mycha(utils::AllSignals& signals);
    void motor(float pwm)
    {
        mMotorL.setPwm(pwm);
        mMotorR.setPwm(-pwm);
    }

  private:
    void connectSignals();
    void initializeMycha();

    void initBle();
    void initLed();
    void initAdc();
    void initMotors();
    void initDistance();

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

    HBridge mHBridge;
    Motor mMotorL;
    Motor mMotorR;

    Encoder mEncoderL;
    Encoder mEncoderR;
};