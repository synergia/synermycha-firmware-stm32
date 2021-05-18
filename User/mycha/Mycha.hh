#pragma once

#include "Led.hh"
#include "controller/TrajectoryGenerator.hh"
#include "motor/HBridge.hh"
#include "motor/Motor.hh"
#include "sensors/DistanceTof.hh"
#include "sensors/Encoder.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"
#include <cmath>

namespace mycha
{
namespace mechanic
{
// all data in SI
constexpr double PI = 3.14159265358979323846;
// meters
constexpr double radius = 0.012;
// meters
constexpr double circumference = 2 * PI * radius;

// seconds
constexpr double controllerPeriod = 0.005;
}  // namespace mechanic

class Mycha : public utils::Observer
{
  public:
    explicit Mycha(utils::AllSignals& signals);
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

    sensors::DistanceTof mSensorL;
    sensors::DistanceTof mSensorFL;
    sensors::DistanceTof mSensorF;
    sensors::DistanceTof mSensorFR;
    sensors::DistanceTof mSensorR;

    mycha::Led mLed1;
    mycha::Led mLed2;
    mycha::Led mLed3;
    mycha::Led mLed4;
    mycha::Led mLed5;

    HBridge mHBridge;
    Motor mMotorL;
    Motor mMotorR;

    sensors::Encoder mEncoderL;
    sensors::Encoder mEncoderR;

    controller::TrajectoryGenerator mTrajectory;
};

}  // namespace mycha