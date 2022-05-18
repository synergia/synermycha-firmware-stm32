#include "Logic.hh"
#include "controller/Pid.hh"
#include "controller/TrajectoryGenerator.hh"
#include "display/Display.hh"
#include "mycha/Mycha.hh"

namespace logic
{

namespace
{

/// @note lack of front left/rigth, for tests
/// when they are present, it seemed to be 0
/// need check it
inline bool areAllDistancesInitialized(const mycha::DistancesData& dist)
{
    return dist.right != 0 and dist.front != 0 and dist.left != 0;
}

void printDistances(utils::AllSignals& signals, const mycha::DistancesData& data)
{
    // signals.displayLogValue.emit("R: %f", (float)data.right, 0, false);
    // signals.displayLogValue.emit("F: %f", (float)data.front, 1, false);
    // signals.displayLogValue.emit("L: %f", (float)data.left, 2, true);
}

}  // namespace

Logic::Logic(utils::AllSignals& signals)
    : mSignals(signals)
    , mActiveController{ControllerType::None}
    , mForwardController{signals}
    , mRotationalController{signals}
    , mCommands{}
{
    // controller::Command cmd;
    // cmd.type       = controller::CommandType::Rotational;
    // cmd.rotational = controller::RotationalCommand{90};
    // mCommands.addCommand(cmd);
    // cmd.type       = controller::CommandType::Rotational;
    // cmd.rotational = controller::RotationalCommand{90};
    // mCommands.addCommand(cmd);
    // cmd.type       = controller::CommandType::Rotational;
    // cmd.rotational = controller::RotationalCommand{-180};
    // mCommands.addCommand(cmd);
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
    mDistancesData = data;
    printDistances(mSignals, data);
}

void Logic::onGetMotorSettings(mycha::MotorsSettings& motorData)
{
    if (isTargetReached())
    {
        resetCurrentControllerAndLogicData();
        loadNewCommand();
    }
    // static double maxAngSpeed = 0;
    // {
    //     double angularSpeed;
    //     mSignals.getGyroZ.emit(angularSpeed);
    //     mAllAngle += angularSpeed * mycha::mechanic::controllerPeriod;
    //     maxAngSpeed = std::max(maxAngSpeed, angularSpeed);
    //     // mSignals.displayLogValue.emit("max omega Z:%f", (double)maxAngSpeed, 0, true);
    //     mSignals.displayLogValue.emit("angle Z:%f", (double)mAllAngle, 0, true);
    // }

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
    if (needToCompensateFrontDistance())
    {
        mActiveController = executeCommand(getCommandToCompensateFrontDistance());
    }
    if (not mCommands.isEmpty())
    {
        mActiveController = executeCommand(mCommands.getNextCommand());
    }
    else
    {
        generateNewCommands();
        mActiveController = ControllerType::None;
    }
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
    default:
        return;
    }
}

void Logic::generateNewCommands()
{
    // avoid reading distances before first data comes
    if (not areAllDistancesInitialized(mDistancesData))
        return;

    const double oneCubeLen = mycha::mechanic::labyrinthCubeSize;

    controller::Command cmd;
    if (sensors::isNoWall(mDistancesData.right))
    {
        cmd.type       = controller::CommandType::Rotational;
        cmd.rotational = controller::RotationalCommand{-90};
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
        cmd.rotational = controller::RotationalCommand{90};
        mCommands.addCommand(cmd);

        cmd.type    = controller::CommandType::Forward;
        cmd.forward = controller::ForwardCommand{oneCubeLen};
        mCommands.addCommand(cmd);
    }
    else
    {
        cmd.type       = controller::CommandType::Rotational;
        cmd.rotational = controller::RotationalCommand{180};
        mCommands.addCommand(cmd);
    }
}

bool Logic::needToCompensateFrontDistance()
{
    // milimeters
    static constexpr double allowedFrontOffset   = 5;
    static constexpr double minAllowedFront      = mycha::mechanic::frontSensorToFrontWallDistance - allowedFrontOffset;
    static constexpr double maxAllowedFront      = mycha::mechanic::frontSensorToFrontWallDistance + allowedFrontOffset;
    static constexpr double cubeSizeInMilimeters = mycha::mechanic::labyrinthCubeSize * 1000;

    // avoid reading distances before first data comes
    if (not areAllDistancesInitialized(mDistancesData))
        return false;

    if (mDistancesData.front > cubeSizeInMilimeters)
        return false;

    return mDistancesData.front < minAllowedFront || mDistancesData.front > maxAllowedFront;
}

controller::Command Logic::getCommandToCompensateFrontDistance()
{
    const double distanceToCompensate = (mDistancesData.front - mycha::mechanic::frontSensorToFrontWallDistance) / 1000;
    controller::Command cmd;
    cmd.type    = controller::CommandType::Forward;
    cmd.forward = controller::ForwardCommand{distanceToCompensate};

    return cmd;
}

}  // namespace logic