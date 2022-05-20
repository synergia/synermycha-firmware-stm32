#include "RotationalController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

namespace
{
constexpr double degreeOverSecondToMeterOverSecond = mycha::mechanic::mouseCircumference / 360.0;
}

RotationalController::RotationalController(utils::AllSignals& signals)
    : mSignals(signals)
    , mTransPid{100, 5, 10}
    , mRotPid{100, 5, 10}
    , mRotTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings RotationalController::getControll(const RotationalControllerInput& input)
{
    mCurrentInput  = input;
    const int sign = mTargetAngle > 0 ? 1 : -1;

    mTime += mycha::mechanic::controllerPeriod;
    mRotTrajectory.calculateTrajectory(mTime);
    const double vRotRef = mRotTrajectory.getVRef();

    const double vTransRef = 0;

    controller::PidIn pidInTrans;
    pidInTrans.measVal = (mCurrentInput.rightWheelSpeed + mCurrentInput.leftWheelSpeed) / 2.0;
    pidInTrans.refVal  = vTransRef;
    controller::PidIn pidInRot;
    pidInRot.measVal = degreeOverSecondToMeterOverSecond * mCurrentInput.angularSpeed;
    // pidInRot.measVal = (mCurrentInput.rightWheelSpeed - mCurrentInput.leftWheelSpeed) / 2.0;
    //  pidInRot.measVal = degreeOverSecondToMeterOverSecond * mCurrentInput.angle;
    pidInRot.refVal = degreeOverSecondToMeterOverSecond * sign * vRotRef;

    const double outTrans = mTransPid.calculate(pidInTrans);
    const double outRot   = mRotPid.calculate(pidInRot);

    // mSignals.displayLogValue.emit("angle:%f", (double)mCurrentInput.angle, 0, false);
    // mSignals.displayLogValue.emit("target:%f", (double)mTargetAngle, 1, false);
    // mSignals.displayLogValue.emit("sRef:%f", (double)mRotTrajectory.getSRef(), 2, false);
    // mSignals.displayLogValue.emit("meas:%f", (double)pidInRot.measVal, 3, true);

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = outTrans - outRot;
    motorData.pwmRightMotor = outTrans + outRot;

    return motorData;
}

void RotationalController::reset()
{
    mTime        = 0.0;
    mTargetAngle = 0.0;
    mRotTrajectory.reset();
    mTransPid.reset();
    mRotPid.reset();
    mCurrentInput = RotationalControllerInput{};
}

void RotationalController::setNewAngle(double angle)
{
    // magic +2 is compensation (mouse always stops around 1 degree to low)
    double angleToWrite;
    if (angle > 0)
    {
        angleToWrite = angle + 2.5;
    }
    else
    {
        angleToWrite = angle - 2.5;
    }
    mTargetAngle = angle;
    // here we set trajectory always positive. In controller is checked in which direction motors should run
    // double targetRoad = (std::abs(angle) / 360.0) * mycha::mechanic::mouseCircumference;
    // mRotTrajectory    = controller::TrajectoryGenerator{targetRoad, 0.4, 1, mycha::mechanic::controllerPeriod};
    mRotTrajectory =
        controller::TrajectoryGenerator{std::abs(angleToWrite), 990, 2475, mycha::mechanic::controllerPeriod};
}

bool RotationalController::isTargetReached() const
{
    static constexpr double delta{0.5};

    const double target = std::abs(mTargetAngle);  // mRotTrajectory.getPathLength();
    return (target - std::abs(mCurrentInput.angle)) <= delta;

    // mouse must reach at least (target - delta) in meters distance or exceed it
    // static constexpr double delta{0.001};

    // const double target       = mRotTrajectory.getPathLength();
    // const bool isLeftReached  = (target - std::abs(mCurrentInput.leftWheelRoad)) <= delta;
    // const bool isRightReached = (target - std::abs(mCurrentInput.rightWheelRoad)) <= delta;
    // return isLeftReached && isRightReached;
}

double RotationalController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{0};

    // addition, because wheels are rotating in reverse direction from each other
    // we want to achieve equal opposite speeds
    double diff = mCurrentInput.rightWheelRoad + mCurrentInput.leftWheelRoad;
    // output should be for one wheel, so divide by 2
    diff /= 2.0;
    return diff * roadDiffCooficient;
}

double RotationalController::getAngleCorrection() const
{
    static constexpr double angleDiffCooficient{2};

    double diff = mTargetAngle - mCurrentInput.angle;
    return diff * angleDiffCooficient;
}

}  // namespace controller