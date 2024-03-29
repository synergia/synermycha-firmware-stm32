#include "ForwardController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

ForwardController::ForwardController(utils::AllSignals& signals)
    : mSignals(signals)
    , mTransPid{100, 5, 10}
    , mRotPid{100, 5, 10}
    , mTransTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings ForwardController::getControll(const ForwardControllerInput& input)
{
    mCurrentInput  = input;
    const int sign = mTargetDistance > 0 ? 1 : -1;

    mTime += mycha::mechanic::controllerPeriod;
    mTransTrajectory.calculateTrajectory(mTime);
    const double vTransRef = mTransTrajectory.getVRef();

    const double vRotRef = 0;

    controller::PidIn pidInTrans;
    pidInTrans.measVal = (mCurrentInput.rightWheelSpeed + mCurrentInput.leftWheelSpeed) / 2.0;
    pidInTrans.refVal  = sign * vTransRef;
    controller::PidIn pidInRot;
    pidInRot.measVal = (mCurrentInput.rightWheelSpeed - mCurrentInput.leftWheelSpeed) / 2.0;
    pidInRot.refVal  = vRotRef;

    const double outTrans = mTransPid.calculate(pidInTrans);
    const double outRot   = mRotPid.calculate(pidInRot);

    // const double roadCorr      = getRoadCorrection();
    const double distancesCorr = getDistancesCorrection();

    // mSignals.displayLogValue.emit("xRef:%f", (double)vTransRef, 0, false);
    // mSignals.displayLogValue.emit("wRef:%f", (double)vRotRef, 1, false);
    // mSignals.displayLogValue.emit("sRef:%f", (double)mTransTrajectory.getSRef(), 2, true);
    // mSignals.displayLogValue.emit("outR:%f", (double)outRot, 3, true);

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = outTrans - outRot + distancesCorr;
    motorData.pwmRightMotor = outTrans + outRot - distancesCorr;

    return motorData;
}

void ForwardController::reset()
{
    mTime           = 0.0;
    mTargetDistance = 0.0;
    mTransTrajectory.reset();
    mTransPid.reset();
    mRotPid.reset();
    mCurrentInput = ForwardControllerInput{};
}

bool ForwardController::isTargetReached() const
{
    // mouse must reach at least (target - delta) in meters distance or exceed it
    static constexpr double delta{0.002};

    const double target       = mTransTrajectory.getPathLength();
    const bool isLeftReached  = (target - std::abs(mCurrentInput.leftWheelRoad)) <= delta;
    const bool isRightReached = (target - std::abs(mCurrentInput.rightWheelRoad)) <= delta;
    return isLeftReached && isRightReached;
}

double ForwardController::getDistancesCorrection() const
{
    const double mouseTransSpeed = (mCurrentInput.rightWheelSpeed + mCurrentInput.leftWheelSpeed) / 2.0;
    static constexpr double distancessDiffCooficient{2};

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

    return diff * mouseTransSpeed * distancessDiffCooficient;
}

double ForwardController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{40};

    double diff = mCurrentInput.rightWheelRoad - mCurrentInput.leftWheelRoad;

    diff /= 2.0;
    return diff * roadDiffCooficient;
}

void ForwardController::setNewDistance(double distance)
{
    mTargetDistance  = distance;
    mTransTrajectory = TrajectoryGenerator{std::abs(mTargetDistance), 0.4, 1, mycha::mechanic::controllerPeriod};
}

}  // namespace controller