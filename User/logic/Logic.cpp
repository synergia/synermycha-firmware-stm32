#include "Logic.hh"
#include "controller/Pid.hh"
#include "controller/TrajectoryGenerator.hh"
#include "display/Display.hh"
#include "mycha/Mycha.hh"

namespace logic
{

Logic::Logic(utils::AllSignals& signals)
    : mSignals(signals)
    , mActiveController{ControllerType::Forward}
    , mForwardController{signals}
    , mRotationalController{}
{
    connectSignals();
}

void Logic::connectSignals()
{
    mSignals.setDrivingData.connect<Logic, &Logic::onSetDrivingData>(*this);
    mSignals.setDistancesData.connect<Logic, &Logic::onSetDistancesData>(*this);
    mSignals.getMotorSettings.connect<Logic, &Logic::onGetMotorSettings>(*this);
}

void Logic::onSetDrivingData(const mycha::DrivingData& data)
{
    mDrivingData = data;
    mAllLeftRoad += data.leftWheelDistance;
    mAllRightRoad += data.rightWheelDistance;
    mLeftSpeed  = data.leftWheelDistance / mycha::mechanic::controllerPeriod;
    mRightSpeed = data.rightWheelDistance / mycha::mechanic::controllerPeriod;
}

void Logic::onSetDistancesData(const mycha::DistancesData& data)
{
    mDistancesData = data;
}

void Logic::onGetMotorSettings(mycha::MotorsSettings& motorData)
{
    static int isok = 0;
    if (isTargetReached())
    {
        executeNewCommand();
        isok++;
    }
    motorData = getDataFromController();

    // mSignals.displayLogValue.emit("R:%f", mAllRightRoad, 0, false);
    // mSignals.displayLogValue.emit("L:%f", mAllLeftRoad, 1, false);
    // mSignals.displayLogValue.emit("isok%f", (double)isok, 2, true);
}

mycha::MotorsSettings Logic::getDataFromController()
{
    switch (mActiveController)
    {
    case ControllerType::Forward:
        return getDataFromForwardController();
    case ControllerType::Rotational:
        return getDataFromRotationalController();
    default:
        return mycha::MotorsSettings{};
    }
}

mycha::MotorsSettings Logic::getDataFromForwardController()
{
    controller::ForwardControllerInput input;
    input.leftDistance   = mDistancesData.left;
    input.rightDistance  = mDistancesData.right;
    input.leftWheelRoad  = mAllLeftRoad;
    input.rightWheelRoad = mAllRightRoad;

    return mForwardController.getControll(input);
}

mycha::MotorsSettings Logic::getDataFromRotationalController()
{
    controller::RotationalControllerInput input;
    input.leftWheelRoad  = mAllLeftRoad;
    input.rightWheelRoad = mAllRightRoad;

    return mRotationalController.getControll(input);
}

bool Logic::isTargetReached() const
{
    switch (mActiveController)
    {
    case ControllerType::Forward:
        return mForwardController.isTargetReached();
    case ControllerType::Rotational:
        return mRotationalController.isTargetReached();
    default:
        return false;
    }
}

void Logic::executeNewCommand()
{
    resetCurrentControllerAndLogicData();
    // here is logic for labirynth solving
    // dummy for now
    mActiveController = ControllerType::Forward;
    mForwardController.setNewTrajectory(
        controller::TrajectoryGenerator{0.18, 0.4, 0.05, mycha::mechanic::controllerPeriod});
}

void Logic::resetCurrentControllerAndLogicData()
{
    mAllLeftRoad  = 0.0;
    mAllRightRoad = 0.0;
    mLeftSpeed    = 0.0;
    mRightSpeed   = 0.0;

    switch (mActiveController)
    {
    case ControllerType::Forward:
        mForwardController.reset();
        break;
    case ControllerType::Rotational:
        mRotationalController.reset();
        break;
    default:
        return;
    }
}

}  // namespace logic