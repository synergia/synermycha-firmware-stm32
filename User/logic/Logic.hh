#pragma once

#include "controller/ForwardController.hh"
#include "controller/RotationalController.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

namespace logic
{

class Logic : public utils::Observer
{
  public:
    Logic(utils::AllSignals& signals);

  private:
    enum class ControllerType
    {
        Forward,
        Rotational
    };

    void connectSignals();
    void onSetDrivingData(const mycha::DrivingData& data);
    void onSetDistancesData(const mycha::DistancesData& data);
    void onGetMotorSettings(mycha::MotorsSettings& motorData);

    mycha::MotorsSettings getDataFromController();
    mycha::MotorsSettings getDataFromForwardController();
    mycha::MotorsSettings getDataFromRotationalController();

    bool isTargetReached() const;
    void executeNewCommand();
    void resetCurrentControllerAndLogicData();

    mycha::DrivingData mDrivingData{};
    mycha::DistancesData mDistancesData{};
    utils::AllSignals& mSignals;

    double mAllLeftRoad  = 0.0;
    double mAllRightRoad = 0.0;
    double mLeftSpeed    = 0.0;
    double mRightSpeed   = 0.0;

    ControllerType mActiveController;
    controller::ForwardController mForwardController;
    controller::RotationalController mRotationalController;
};

}  // namespace logic