#pragma once

#include "sensors/VL53L0X/core/inc/vl53l0x_api.h"
#include "utils/HalUtils.hh"
#include <limits>

namespace sensors
{

namespace calibration
{
constexpr int32_t calibrationDistanceInMicroMeters = 100000;
// outputs from VL53L0X_PerformOffsetCalibration()
constexpr int32_t outLeft       = -95000;
constexpr int32_t outFrontLeft  = -96000;
constexpr int32_t outFront      = -82000;
constexpr int32_t outFrontRight = -94000;
constexpr int32_t outRight      = -101000;

constexpr int32_t offsetLeft       = calibrationDistanceInMicroMeters + outLeft;
constexpr int32_t offsetFrontLeft  = calibrationDistanceInMicroMeters + outFrontLeft;
constexpr int32_t offsetFront      = calibrationDistanceInMicroMeters + outFront;
constexpr int32_t offsetFrontRight = calibrationDistanceInMicroMeters + outFrontRight;
constexpr int32_t offsetRight      = calibrationDistanceInMicroMeters + outRight;
}  // namespace calibration

namespace address
{
constexpr uint8_t left       = 42;
constexpr uint8_t frontLeft  = 44;
constexpr uint8_t front      = 46;
constexpr uint8_t frontRight = 48;
constexpr uint8_t right      = 50;
}  // namespace address

bool isNoWall(uint16_t sensorData);
bool isWall(uint16_t sensorData);

class DistanceTof
{
    using TofAddress = uint8_t;
    using ToFOffset  = int32_t;

  public:
    DistanceTof(GpioPort port, GpioPin pin, TofAddress address, ToFOffset offset);
    void initialize();

    uint16_t getLastMeasurement() const;
    uint16_t readDistance();

    static constexpr uint16_t INVALID_VALUE = std::numeric_limits<uint16_t>::max();

  private:
    GpioPort mPort;
    GpioPin mPin;
    TofAddress mAddress;
    ToFOffset mOffset;

    VL53L0X_Dev_t vl53l0x_c;  // center module
    VL53L0X_DEV mDev = &vl53l0x_c;

    uint16_t mLastMeasurement;
};

}  // namespace sensors