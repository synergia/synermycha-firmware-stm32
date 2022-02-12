#include "RotationalController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

RotationalController::RotationalController(utils::AllSignals& signals)
    : mSignals(signals)
    , mLeftPid{1000, 20, 10}
    , mRightPid{1000, 20, 10}
    , mTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings RotationalController::getControll(const RotationalControllerInput& input)
{
    mCurrentInput = input;

    const int signRight = mTargetAngle >= 0 ? 1 : -1;
    const int signLeft  = -signRight;

    mTime += mycha::mechanic::controllerPeriod;
    mTrajectory.calculateTrajectory(mTime);
    const double sRef = mTrajectory.getSRef();

    controller::PidIn pidInRight;
    pidInRight.measVal    = mCurrentInput.rightWheelRoad;
    pidInRight.refVal     = sRef * signRight;
    const double outRight = mRightPid.calculate(pidInRight);
    controller::PidIn pidInLeft;
    pidInLeft.measVal    = mCurrentInput.leftWheelRoad;
    pidInLeft.refVal     = sRef * signLeft;
    const double outLeft = mLeftPid.calculate(pidInLeft);

    mSignals.displayLogValue.emit("alfa:%f", mCurrentInput.angle, 1, false);
    mSignals.displayLogValue.emit("sRef:%f", sRef, 2, false);

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = outLeft;
    motorData.pwmRightMotor = outRight;

    return motorData;
}

void RotationalController::reset()
{
    mTime        = 0.0;
    mTargetAngle = 0.0;
    mTrajectory.reset();
    mLeftPid.reset();
    mRightPid.reset();
    mCurrentInput = RotationalControllerInput{};
}

void RotationalController::setNewAngle(double angle)
{
    // magic +2 is compensation (mouse always stops around 1 degree to low)
    if (angle > 0)
    {
        mTargetAngle = angle + 1;
    }
    else
    {
        mTargetAngle = angle - 1;
    }
    // here we set trajectory always positive. In controller is checked in which direction motors should run
    double targetRoad = (std::abs(angle) / 360.0) * mycha::mechanic::mouseCircumference;
    mTrajectory       = controller::TrajectoryGenerator{targetRoad, 0.4, 0.1, mycha::mechanic::controllerPeriod};
}

bool RotationalController::isTargetReached() const
{
    // mouse must reach at least (targetAngle - delta) in degrees angle or exceed it
    static constexpr double delta{0.5};

    return (std::abs(mTargetAngle) - std::abs(mCurrentInput.angle)) <= delta;
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