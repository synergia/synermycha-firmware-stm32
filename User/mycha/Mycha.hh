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
constexpr double wheelRadius = 0.012;

// meters
constexpr double mouseRadius = 0.047 / 2.0;

// meters
constexpr double wheelCircumference = 2 * PI * wheelRadius;

// seconds
constexpr double controllerPeriod = 0.005;

// ticks
// WARNING: experimental changes. Firstly was 32 * 160 !!!!!!!!!!!!!!!!!!!!!!
constexpr uint32_t tickPerRevolution = 16 * 160;

// const for calculating angular speed for givent ticks
constexpr double ticksToAngularSpeedMultipler = (2 * PI / tickPerRevolution) / controllerPeriod;

}  // namespace mechanic

struct MouseData
{
    double angularSpeed;
    double transSpeed;
    double angularDistance;
    double transDistance;
};

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
    MouseData getMouseData();

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