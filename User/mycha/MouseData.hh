#pragma once

#include <cstdint>

namespace mycha
{

struct DrivingData
{
    double leftWheelDistance;
    double rightWheelDistance;
};

struct DistancesData
{
    uint16_t left;
    uint16_t frontLeft;
    uint16_t front;
    uint16_t frontRight;
    uint16_t right;
};

struct MotorsSettings
{
    float pwmLeftMotor;
    float pwmRightMotor;
};

}  // namespace mycha