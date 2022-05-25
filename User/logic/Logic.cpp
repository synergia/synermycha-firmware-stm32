#include "Logic.hh"
#include "controller/Pid.hh"
#include "controller/TrajectoryGenerator.hh"
#include "display/Display.hh"
#include "mycha/Mycha.hh"
#include "utils/Timer.hh"

namespace logic
{

namespace
{

void printDistances(utils::AllSignals& signals, const mycha::DistancesData& data)
{
    signals.displayLogValue.emit("R: %f", (float)data.right, 0, false);
    signals.displayLogValue.emit("F: %f", (float)data.front, 1, false);
    signals.displayLogValue.emit("L: %f", (float)data.left, 2, true);
}

}  // namespace

Logic::Logic(utils::AllSignals& signals)
    : mSignals(signals)
    , mActiveController{ControllerType::None}
    , mForwardController{signals}
    , mRotationalController{signals}
    , mCommands{}
    , mMaze{signals}
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

    // mSignals.displayLogValue.emit("allR:%f", (double)mAllRightRoad, 3, false);
}

void Logic::onSetDistancesData(const mycha::DistancesData& data)
{
    static bool isFirstMazeUpdateDone = false;

    mDistancesData = data;
    if (not isFirstMazeUpdateDone)
    {
        mMaze.updateWallsAtStartup(mDistancesData);
        isFirstMazeUpdateDone = true;
    }

    // printDistances(mSignals, data);
}

void Logic::onGetMotorSettings(mycha::MotorsSettings& motorData)
{
    static int targetCounter = 0;

    if (isTargetReached())
    {
        targetCounter++;
        mMaze.updateMouseAndMazeState(mLastCommand, mDistancesData);
        mMaze.drawMazeWeights();
        // mMaze.drawMazeWalls();
        mSignals.mazeReadyToShow.emit();
        resetCurrentControllerAndLogicData();
        if (not mMaze.isMouseInFinishPoint())
            loadNewCommand();
    }

    // mSignals.displayLogValue.emit("R: %f", (float)targetCounter, 0, true);

    motorData = getDataFromController();
}

mycha::MotorsSettings Logic::getDataFromController()
{
    switch (mActiveController)
    {
    case ControllerType::Forward:
        return getDataFromForwardController();
    case ControllerType::Rotational:
        return getDataFromRotationalController();
    case ControllerType::None:
    default:
        return mycha::MotorsSettings{};
    }
}

mycha::MotorsSettings Logic::getDataFromForwardController()
{
    controller::ForwardControllerInput input{};
    input.leftDistance    = mDistancesData.left;
    input.rightDistance   = mDistancesData.right;
    input.leftWheelRoad   = mAllLeftRoad;
    input.rightWheelRoad  = mAllRightRoad;
    input.leftWheelSpeed  = mLeftSpeed;
    input.rightWheelSpeed = mRightSpeed;

    return mForwardController.getControll(input);
}

mycha::MotorsSettings Logic::getDataFromRotationalController()
{
    double angularSpeed;
    mSignals.getGyroZ.emit(angularSpeed);
    mAllAngle += angularSpeed * mycha::mechanic::controllerPeriod;

    controller::RotationalControllerInput input{};
    input.leftWheelRoad   = mAllLeftRoad;
    input.rightWheelRoad  = mAllRightRoad;
    input.leftWheelSpeed  = mLeftSpeed;
    input.rightWheelSpeed = mRightSpeed;
    input.angle           = mAllAngle;
    input.angularSpeed    = angularSpeed;

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
    case ControllerType::None:
    default:
        return true;
    }
}

void Logic::loadNewCommand()
{
    // if (needToCompensateFrontDistance())
    // {
    //     mLastCommand = getCommandToCompensateFrontDistance();
    // }
    // else if (not mCommands.isEmpty())
    if (not mCommands.isEmpty())
    {
        mLastCommand = mCommands.getNextCommand();
    }
    else
    {
        generateNewCommands();
        mLastCommand = mCommands.getNextCommand();
    }

    mActiveController = executeCommand(mLastCommand);
}

Logic::ControllerType Logic::executeCommand(const controller::Command& command)
{
    ControllerType type = ControllerType::None;

    using controller::CommandType;
    switch (command.type)
    {
    case CommandType::Forward:
        executeCommandOnForwardController(command.forward);
        type = ControllerType::Forward;
        break;
    case CommandType::Rotational:
        executeCommandOnRotationalController(command.rotational);
        type = ControllerType::Rotational;
        break;
    default:
        type = ControllerType::None;
    }

    return type;
}

void Logic::executeCommandOnForwardController(const controller::ForwardCommand& command)
{
    mForwardController.setNewDistance(command.length);
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
    case ControllerType::None:
        return;
    }

    mActiveController = ControllerType::None;
    mLastCommand      = controller::Command{};
}

void Logic::generateNewCommands()
{
    const double oneCubeLen = mycha::mechanic::labyrinthCubeSize;
    controller::Command cmd{};
    if (sensors::isNoWall(mDistancesData.right))
    {
        cmd.type       = controller::CommandType::Rotational;
        cmd.rotational = controller::RotationalCommand{controller::RotationDir::Right};
        mCommands.addCommand(cmd);

        cmd.type    = controller::CommandType::Forward;
        cmd.forward = controller::ForwardCommand{oneCubeLen};
        mCommands.addCommand(cmd);
    }
    else if (sensors::isNoWall(mDistancesData.front))
    {
        cmd.type    = controller::CommandType::Forward;
        cmd.forward = controller::ForwardCommand{oneCubeLen};
        mCommands.addCommand(cmd);
    }
    else if (sensors::isNoWall(mDistancesData.left))
    {
        cmd.type       = controller::CommandType::Rotational;
        cmd.rotational = controller::RotationalCommand{controller::RotationDir::Left};
        mCommands.addCommand(cmd);

        cmd.type    = controller::CommandType::Forward;
        cmd.forward = controller::ForwardCommand{oneCubeLen};
        mCommands.addCommand(cmd);
    }
    else
    {
        cmd.type       = controller::CommandType::Rotational;
        cmd.rotational = controller::RotationalCommand{controller::RotationDir::TurnBack};
        mCommands.addCommand(cmd);
    }
}

bool Logic::needToCompensateFrontDistance()
{
    // milimeters
    static constexpr double allowedFrontOffset   = 10;
    static constexpr double minAllowedFront      = mycha::mechanic::frontSensorToFrontWallDistance - allowedFrontOffset;
    static constexpr double maxAllowedFront      = mycha::mechanic::frontSensorToFrontWallDistance + allowedFrontOffset;
    static constexpr double cubeSizeInMilimeters = mycha::mechanic::labyrinthCubeSize * 1000;

    if (mDistancesData.front > cubeSizeInMilimeters)
        return false;

    return mDistancesData.front < minAllowedFront || mDistancesData.front > maxAllowedFront;
}

controller::Command Logic::getCommandToCompensateFrontDistance()
{
    const double distanceToCompensate = (mDistancesData.front - mycha::mechanic::frontSensorToFrontWallDistance) / 1000;
    controller::Command cmd{};
    cmd.type    = controller::CommandType::Forward;
    cmd.forward = controller::ForwardCommand{distanceToCompensate};

    return cmd;
}

}  // namespace logic