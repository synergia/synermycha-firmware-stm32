#include "RotationalController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

RotationalController::RotationalController(utils::AllSignals& signals)
    : mSignals(signals)
    , mLeftPid{1000, 20, 10}
    , mRightPid{1000, 20, 10}
    , mAnglePid{1, 0.02, 1}
    , mTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings RotationalController::getControll(const RotationalControllerInput& input)
{
    mCurrentInput = input;

    const int signLeft  = mTargetAngle >= 0 ? -1 : 1;
    const int signRight = mTargetAngle >= 0 ? 1 : -1;

    mTime += mycha::mechanic::controllerPeriod;
    mTrajectory.calculateTrajectory(mTime);
    const double sRef = mTrajectory.getSRef();

    controller::PidIn pidInAngle;
    pidInAngle.measVal = mCurrentInput.angle;
    pidInAngle.refVal  = sRef;

    // controller::PidIn pidInRight;
    // pidInRight.measVal = mCurrentInput.rightWheelRoad;
    // pidInRight.refVal  = sRef * signRight;
    // controller::PidIn pidInLeft;
    // pidInLeft.measVal = mCurrentInput.leftWheelRoad;
    // pidInLeft.refVal  = sRef * signLeft;

    // const double outLeft  = mLeftPid.calculate(pidInLeft);
    // const double outRight = mRightPid.calculate(pidInRight);
    const double outAngle = mAnglePid.calculate(pidInAngle);

    mSignals.displayLogValue.emit("R:%f", mCurrentInput.rightWheelRoad, 0, false);
    mSignals.displayLogValue.emit("alfa:%f", mCurrentInput.angle, 1, false);
    mSignals.displayLogValue.emit("sRef:%f", sRef, 2, false);

    const double roadCorr  = getRoadCorrection();
    const double angleCorr = getAngleCorrection();

    mycha::MotorsSettings motorData;
    motorData.pwmLeftMotor  = -outAngle + roadCorr;  // outLeft + roadCorr + angleCorr;
    motorData.pwmRightMotor = outAngle + roadCorr;   // outRight - roadCorr + angleCorr;

    return motorData;
}

void RotationalController::reset()
{
    mTime        = 0.0;
    mTargetAngle = 0.0;
    mTrajectory.reset();
    mLeftPid.reset();
    mRightPid.reset();
    mAnglePid.reset();
    mCurrentInput = RotationalControllerInput{};
}

void RotationalController::setNewAngle(double angle)
{
    mTargetAngle = angle;
    // double targetRoad = (angle / 360.0) * mycha::mechanic::mouseCircumference;
    // mTrajectory       = controller::TrajectoryGenerator{targetRoad, 0.4, 0.05, mycha::mechanic::controllerPeriod};
    mTrajectory = controller::TrajectoryGenerator{mTargetAngle, 90, 50, mycha::mechanic::controllerPeriod};
}

bool RotationalController::isTargetReached() const
{
    // mouse must reach at least (targetAngle - delta) in degrees angle or exceed it
    static constexpr double delta{0.5};

    return (mTargetAngle - mCurrentInput.angle) <= delta;
}

double RotationalController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{45};

    double diff = mCurrentInput.rightWheelRoad - mCurrentInput.leftWheelRoad;
    return diff * roadDiffCooficient;
}

double RotationalController::getAngleCorrection() const
{
    static constexpr double angleDiffCooficient{2};

    double diff = mTargetAngle - mCurrentInput.angle;
    return diff * angleDiffCooficient;
}

}  // namespace controller