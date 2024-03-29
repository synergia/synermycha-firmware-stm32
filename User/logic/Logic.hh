#pragma once

#include "Maze.hh"
#include "controller/CommandQueue.hh"
#include "controller/ForwardController.hh"
#include "controller/RotationalController.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"
#include "utils/LoggingSystem.hh"
#include "utils/Observer.hh"

namespace logic
{

class Logic : public utils::Observer
{
  public:
    explicit Logic(utils::AllSignals& signals, utils::LoggingSystem& logger);

  private:
    enum class ControllerType
    {
        None,
        Forward,
        Rotational
    };

    enum class MousePhase
    {
        WaitingForButtonEnterToSearchRun,
        WaitingForSearchRunStart,
        SearchRun,
        BackToStart,
        WaitingForButtonEnterToFastRun,
        WaitingForFastRunStart,
        FastRun,
        RightHand,
        TestMode
    };

    void connectSignals();
    void onButtonEnter();
    void onSetDrivingData(const mycha::DrivingData& data);
    void onSetDistancesData(const mycha::DistancesData& data);
    void onGetMotorSettings(mycha::MotorsSettings& motorData);

    mycha::MotorsSettings getDataFromController();
    mycha::MotorsSettings getDataFromForwardController();
    mycha::MotorsSettings getDataFromRotationalController();

    bool isTargetReached() const;
    void loadNewCommand();
    ControllerType executeCommand(const controller::Command& command);
    void executeCommandOnForwardController(const controller::ForwardCommand& command);
    void executeCommandOnRotationalController(const controller::RotationalCommand& command);
    void resetCurrentControllerAndLogicData();

    void generateNewCommands();

    bool needToCompensateFrontDistance();
    controller::Command getCommandToCompensateFrontDistance();

    void msCallback();
    void setNewPhase(MousePhase newPhase);
    const char* toCString(MousePhase phase);

    mycha::DrivingData mDrivingData{};
    mycha::DistancesData mDistancesData{};
    utils::AllSignals& mSignals;
    utils::LoggingSystem& mLogger;

    double mAllLeftRoad  = 0.0;
    double mAllRightRoad = 0.0;
    double mLeftSpeed    = 0.0;
    double mRightSpeed   = 0.0;
    double mAllAngle     = 0.0;

    ControllerType mActiveController;
    controller::ForwardController mForwardController;
    controller::RotationalController mRotationalController;
    controller::CommandQueue mCommands;
    controller::Command mLastCommand{};

    Maze mMaze;

    MousePhase mMousePhase;

    uint32_t mMsFromClickEnter = 0;
};

}  // namespace logic