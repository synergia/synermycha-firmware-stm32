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

    mSignals.displayLogValue.emit("R:%f", mCurrentInput.rightWheelRoad, 0, false);
    mSignals.displayLogValue.emit("L:%f", mCurrentInput.leftWheelRoad, 1, false);

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
}

bool ForwardController::isTargetReached() const
{
    // mouse must reach at least (target - delta) in meters distance or exceed it
    static constexpr double delta{0.002};

    const double finish       = mTrajectory.getPathLength();
    const bool isLeftReached  = (finish - mCurrentInput.leftWheelRoad) <= delta;
    const bool isRightReached = (finish - mCurrentInput.rightWheelRoad) <= delta;
    mSignals.displayLogValue.emit("finish:%f", (double)finish, 2, false);
    mSignals.displayLogValue.emit("left:%f", (double)mCurrentInput.leftWheelRoad, 3, true);
    return isLeftReached && isRightReached;
}

double ForwardController::getDistancesCorrection() const
{
    static constexpr double distancessDiffCooficient{0.1};

    double diff = 0.0;
    if (mCurrentInput.rightDistance < 120)
    {
        diff = mCurrentInput.rightDistance - mCurrentInput.leftDistance;
    }

    return diff * distancessDiffCooficient;
}

double ForwardController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{20};

    double diff = mCurrentInput.rightWheelRoad - mCurrentInput.leftWheelRoad;
    return diff * roadDiffCooficient;
}

void ForwardController::setNewTrajectory(const TrajectoryGenerator& trajectory)
{
    mTrajectory = trajectory;
}

}  // namespace controller