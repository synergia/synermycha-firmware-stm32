#include "ForwardController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

ForwardController::ForwardController(utils::AllSignals& signals)
    : mSignals(signals)
    , mTransPid{100, 10, 0}
    , mRotPid{100, 10, 0}
    , mTransTrajectory{}
    , mCurrentInput{}
    , mRotTrajectory{controller::TrajectoryGenerator{mycha::mechanic::mouseCircumference / 4, 0.4, 1,
                                                     mycha::mechanic::controllerPeriod}}
{
}

mycha::MotorsSettings ForwardController::getControll(const ForwardControllerInput& input)
{
    mCurrentInput        = input;
    static double mTime2 = 0.0;

    mTime += mycha::mechanic::controllerPeriod;
    mTransTrajectory.calculateTrajectory(mTime);
    const double vTransRef = mTransTrajectory.getVRef();

    double vRotRef = 0;
    if (mTransTrajectory.getSRef() > 0.64)
    {
        mRotTrajectory.calculateTrajectory(mTime2);
        vRotRef = mRotTrajectory.getVRef();
        mTime2 += mycha::mechanic::controllerPeriod;
    }
    else
    {
        vRotRef = 0;
    }

    controller::PidIn pidInTrans;
    pidInTrans.measVal = (mCurrentInput.rightWheelSpeed + mCurrentInput.leftWheelSpeed) / 2.0;
    pidInTrans.refVal  = vTransRef;
    controller::PidIn pidInRot;
    pidInRot.measVal = (mCurrentInput.rightWheelSpeed - mCurrentInput.leftWheelSpeed) / 2.0;
    pidInRot.refVal  = vRotRef;

    const double outTrans = mTransPid.calculate(pidInTrans);
    const double outRot   = mRotPid.calculate(pidInRot);

    // const double roadCorr      = getRoadCorrection();
    // const double distancesCorr = getDistancesCorrection();

    mSignals.displayLogValue.emit("xRef:%f", (double)vTransRef, 0, false);
    mSignals.displayLogValue.emit("wRef:%f", (double)vRotRef, 1, false);
    mSignals.displayLogValue.emit("sRef:%f", (double)mTransTrajectory.getSRef(), 2, true);
    // mSignals.displayLogValue.emit("outR:%f", (double)outRot, 3, true);

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = outTrans - outRot;
    motorData.pwmRightMotor = outTrans + outRot;

    return motorData;
}

void ForwardController::reset()
{
    mTime = 0.0;
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
    mTransTrajectory = trajectory;
}

}  // namespace controller