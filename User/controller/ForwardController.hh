#pragma once

#include "Pid.hh"
#include "TrajectoryGenerator.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"

namespace controller
{

struct ForwardControllerInput
{
    double leftWheelRoad;
    double rightWheelRoad;
    double leftDistance;
    double rightDistance;
};

class ForwardController
{
  public:
    ForwardController(utils::AllSignals& signals);

    mycha::MotorsSettings getControll(const ForwardControllerInput& input);
    void reset();
    bool isTargetReached() const;
    void setNewTrajectory(const TrajectoryGenerator& trajectory);

  private:
    double getDistancesCorrection() const;
    double getRoadCorrection() const;

    utils::AllSignals& mSignals;

    Pid mLeftPid;
    Pid mRightPid;
    TrajectoryGenerator mTrajectory;
    ForwardControllerInput mCurrentInput;

    double mTime              = 0.0;
    double mAllLeftWheelRoad  = 0.0;
    double mAllRightWheelRoad = 0.0;
};

}  // namespace controller