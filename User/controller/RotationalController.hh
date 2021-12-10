#pragma once

#include "Pid.hh"
#include "TrajectoryGenerator.hh"
#include "mycha/MouseData.hh"

namespace controller
{

struct RotationalControllerInput
{
    double leftWheelRoad;
    double rightWheelRoad;
    double angle;
};

class RotationalController
{
  public:
    RotationalController();

    mycha::MotorsSettings getControll(const RotationalControllerInput& input);
    void reset();
    bool isTargetReached() const;

  private:
    double getRoadCorrection() const;

    Pid mLeftPid;
    Pid mRightPid;
    TrajectoryGenerator mTrajectory;
    RotationalControllerInput mCurrentInput;

    double mTime = 0.0;
};

}  // namespace controller