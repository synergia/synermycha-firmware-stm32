#include "RotationalController.hh"
#include "mycha/Mycha.hh"

namespace controller
{

RotationalController::RotationalController()
    : mLeftPid{100, 0, 10}
    , mRightPid{100, 0, 10}
    , mTrajectory{}
    , mCurrentInput{}
{
}

mycha::MotorsSettings RotationalController::getControll(const RotationalControllerInput& input)
{
    // mCurrentInput = input;

    // mTime += mycha::mechanic::controllerPeriod;
    // mTrajectory.calculateTrajectory(mTime);
    // const double sRef = mTrajectory.getSRef();

    // controller::PidIn pidInRight;
    // pidInRight.measVal = mCurrentInput.rightWheelRoad;
    // pidInRight.refVal  = sRef;
    // controller::PidIn pidInLeft;
    // pidInLeft.measVal = mCurrentInput.leftWheelRoad;
    // pidInLeft.refVal  = sRef;

    // const double outLeft  = mLeftPid.calculate(pidInLeft);
    // const double outRight = mRightPid.calculate(pidInRight);

    // const double roadCorr = getRoadCorrection();

    // mycha::MotorsSettings motorData;
    // motorData.pwmLeftMotor  = outLeft + roadCorr;
    // motorData.pwmRightMotor = outRight - roadCorr;

    // return motorData;
    return mycha::MotorsSettings{};
}

void RotationalController::reset()
{
    mTime = 0.0;
    mTrajectory.reset();
    mLeftPid.reset();
    mRightPid.reset();
}

double RotationalController::getRoadCorrection() const
{
    static constexpr double roadDiffCooficient{20};

    double diff = mCurrentInput.rightWheelRoad - mCurrentInput.leftWheelRoad;
    return diff * roadDiffCooficient;
}

bool RotationalController::isTargetReached() const
{
    // dummy
    return true;
}

}  // namespace controller