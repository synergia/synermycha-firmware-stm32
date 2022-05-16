#pragma once

#include "Pid.hh"
#include "TrajectoryGenerator.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"

namespace controller
{

struct ForwardControllerInput
{
    // whole road which mouse went
    double leftWheelRoad;
    double rightWheelRoad;
    double leftWheelSpeed;
    double rightWheelSpeed;
    double leftDistance;
    double rightDistance;
};

class ForwardController
{
  public:
    explicit ForwardController(utils::AllSignals& signals);

    mycha::MotorsSettings getControll(const ForwardControllerInput& input);
    void reset();
    bool isTargetReached() const;
    void setNewTrajectory(const TrajectoryGenerator& trajectory);

  private:
    double getDistancesCorrection() const;
    double getRoadCorrection() const;

    utils::AllSignals& mSignals;

    Pid mTransPid;
    Pid mRotPid;
    TrajectoryGenerator mTransTrajectory;
    ForwardControllerInput mCurrentInput;
    TrajectoryGenerator mRotTrajectory;

    double mTime = 0.0;
};

}  // namespace controller