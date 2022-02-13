#include "ForwardController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

ForwardController::ForwardController(utils::AllSignals& signals)
    : mSignals(signals)
    , mLeftPid{1000, 20, 10}
    , mRightPid{1000, 20, 10}
    , mTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings ForwardController::getControll(const ForwardControllerInput& input)
{
    mCurrentInput = input;

    mTime += mycha::mechanic::controllerPeriod;
    mTrajectory.calculateTrajectory(mTime);
    const double sRef = mTrajectory.getSRef();

    controller::PidIn pidInRight;
    pidInRight.measVal = mCurrentInput.rightWheelRoad;
    pidInRight.refVal  = sRef;
    controller::PidIn pidInLeft;
    pidInLeft.measVal = mCurrentInput.leftWheelRoad;
    pidInLeft.refVal  = sRef;

    const double outLeft  = mLeftPid.calculate(pidInLeft);
    const double outRight = mRightPid.calculate(pidInRight);

    const double roadCorr      = getRoadCorrection();
    const double distancesCorr = getDistancesCorrection();

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = outLeft + roadCorr + distancesCorr;
    motorData.pwmRightMotor = outRight - roadCorr - distancesCorr;

    return motorData;
}

void ForwardController::reset()
{
    mTime = 0.0;
    mTrajectory.reset();
    mLeftPid.reset();
    mRightPid.reset();
    mCurrentInput = ForwardControllerInput{};
}

bool ForwardController::isTargetReached() const
{
    // mouse must reach at least (target - delta) in meters distance or exceed it
    static constexpr double delta{0.002};

    const double target       = mTrajectory.getPathLength();
    const bool isLeftReached  = (target - mCurrentInput.leftWheelRoad) <= delta;
    const bool isRightReached = (target - mCurrentInput.rightWheelRoad) <= delta;
    return isLeftReached && isRightReached;
}

double ForwardController::getDistancesCorrection() const
{
    static constexpr double distancessDiffCooficient{0.32};

    double diff = 0.0;
    // walls on both sides
    if (sensors::isWall(mCurrentInput.rightDistance) and sensors::isWall(mCurrentInput.leftDistance))
    {
        diff = mCurrentInput.rightDistance - mCurrentInput.leftDistance;
        diff /= 2.0;
    }
    // wall only on right
    else if (sensors::isWall(mCurrentInput.rightDistance))
    {
        diff = mCurrentInput.rightDistance - mycha::mechanic::sideSensorToSideWallDistance;
    }
    // wall only on left
    else if (sensors::isWall(mCurrentInput.leftDistance))
    {
        diff = mycha::mechanic::sideSensorToSideWallDistance - mCurrentInput.leftDistance;
    }

    return diff * distancessDiffCooficient;
}

double ForwardController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{40};

    double diff = mCurrentInput.rightWheelRoad - mCurrentInput.leftWheelRoad;

    diff /= 2.0;
    return diff * roadDiffCooficient;
}

void ForwardController::setNewTrajectory(const TrajectoryGenerator& trajectory)
{
    mTrajectory = trajectory;
}

}  // namespace controller