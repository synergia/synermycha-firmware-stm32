#pragma once

#include "Pid.hh"
#include "TrajectoryGenerator.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"

namespace controller
{

struct RotationalControllerInput
{
    // whole angle which mouse rotated
    double angle;
    // whole road which mouse went (each wheel in opposite direction)
    double leftWheelRoad;
    double rightWheelRoad;
    double leftWheelSpeed;
    double rightWheelSpeed;
};

class RotationalController
{
  public:
    explicit RotationalController(utils::AllSignals& signals);

    mycha::MotorsSettings getControll(const RotationalControllerInput& input);
    void reset();
    bool isTargetReached() const;
    void setNewAngle(double angle);

  private:
    double getRoadCorrection() const;
    double getAngleCorrection() const;

    utils::AllSignals& mSignals;

    Pid mLeftPid;
    Pid mRightPid;
    TrajectoryGenerator mTrajectory;
    RotationalControllerInput mCurrentInput;
    double mTargetAngle = 0.0;

    double mTime = 0.0;
};

}  // namespace controller