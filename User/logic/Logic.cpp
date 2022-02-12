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
    , mRotationalController{signals}
    , mCommands{}
{
    controller::Command cmd;

    cmd.type    = controller::CommandType::Forward;
    cmd.forward = controller::ForwardCommand{1};
    mCommands.addCommand(cmd);

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
    static int cnt;
    if (isTargetReached())
    {
        loadNewCommand();
        ++cnt;
    }

    motorData = mValidCommand ? getDataFromController() : mycha::MotorsSettings{};

    mSignals.displayLogValue.emit("cnt:%f", cnt, 3, true);
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
    double angularSpeed;
    mSignals.getGyroZ.emit(angularSpeed);
    mAllAngle += angularSpeed * mycha::mechanic::controllerPeriod;

    controller::RotationalControllerInput input;
    input.leftWheelRoad  = mAllLeftRoad;
    input.rightWheelRoad = mAllRightRoad;
    input.angle          = mAllAngle;

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

void Logic::loadNewCommand()
{
    resetCurrentControllerAndLogicData();
    // here is logic for labirynth solving
    // dummy for now
    if (not mCommands.isEmpty())
    {
        mValidCommand = true;
        executeCommand(mCommands.getNextCommand());
    }
    else
    {
        mValidCommand = false;
    }
}

void Logic::executeCommand(const controller::Command& command)
{
    using controller::CommandType;
    switch (command.type)
    {
    case CommandType::Forward:
        executeCommandOnForwardController(command.forward);
        mActiveController = ControllerType::Forward;
        break;
    case CommandType::Rotational:
        executeCommandOnRotationalController(command.rotational);
        mActiveController = ControllerType::Rotational;
        break;
    default:
        return;
    }
}

void Logic::executeCommandOnForwardController(const controller::ForwardCommand& command)
{
    static constexpr double cubeSize{0.18};

    auto xFinish = cubeSize * command.nrOfCubes;
    mForwardController.setNewTrajectory(
        controller::TrajectoryGenerator{xFinish, 0.5, 0.4, mycha::mechanic::controllerPeriod});
}

void Logic::executeCommandOnRotationalController(const controller::RotationalCommand& command)
{
    mRotationalController.setNewAngle(command.angle);
}

void Logic::resetCurrentControllerAndLogicData()
{
    mAllLeftRoad  = 0.0;
    mAllRightRoad = 0.0;
    mLeftSpeed    = 0.0;
    mRightSpeed   = 0.0;
    mAllAngle     = 0.0;

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