#include "DistanceTof.hh"
#include <i2c.h>

namespace sensors
{

namespace
{
// empiric value, in milimeters
constexpr uint16_t noWallDistance{120};
}  // namespace

bool isNoWall(uint16_t sensorData)
{
    const bool isMeasurementValid = sensorData != sensors::DistanceTof::INVALID_VALUE;
    const bool noWall             = sensorData > noWallDistance;

    return isMeasurementValid and noWall;
}

bool isWall(uint16_t sensorData)
{
    return sensorData <= noWallDistance;
}

DistanceTof::DistanceTof(GpioPort port, GpioPin pin, TofAddress address, ToFOffset offset)
    : mPort(port)
    , mPin(pin)
    , mAddress(address)
    , mOffset(offset)
    , mLastMeasurement(0)
{
    mDev->I2cHandle  = &hi2c1;
    mDev->I2cDevAddr = 0x52;
}

void DistanceTof::initialize()
{
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    gpioWrite(mPort, mPin, false);  // Disable XSHUT
    WaitMs(20);
    gpioWrite(mPort, mPin, true);  // Enable XSHUT
    WaitMs(20);
    VL53L0X_SetDeviceAddress(mDev, mAddress);
    mDev->I2cDevAddr = mAddress;

    VL53L0X_WaitDeviceBooted(mDev);
    VL53L0X_DataInit(mDev);
    VL53L0X_StaticInit(mDev);
    VL53L0X_PerformRefCalibration(mDev, &VhvSettings, &PhaseCal);
    VL53L0X_PerformRefSpadManagement(mDev, &refSpadCount, &isApertureSpads);
    VL53L0X_SetDeviceMode(mDev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);

    // Enable/Disable Sigma and Signal check
    VL53L0X_SetLimitCheckEnable(mDev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    VL53L0X_SetLimitCheckEnable(mDev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    VL53L0X_SetLimitCheckValue(mDev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1 * 65536));
    VL53L0X_SetLimitCheckValue(mDev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60 * 65536));
    VL53L0X_SetMeasurementTimingBudgetMicroSeconds(mDev, 33000);
    VL53L0X_SetVcselPulsePeriod(mDev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
    VL53L0X_SetVcselPulsePeriod(mDev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);

    VL53L0X_SetOffsetCalibrationDataMicroMeter(mDev, mOffset);

    VL53L0X_StartMeasurement(mDev);
}

uint16_t DistanceTof::getLastMeasurement() const
{
    return mLastMeasurement;
}

uint16_t DistanceTof::readDistance()
{
    VL53L0X_RangingMeasurementData_t rangingData;
    uint8_t isDataReady{false};

    // it should probably not happen (readings are 25Hz, this sensor is doing 33Hz)
    VL53L0X_GetMeasurementDataReady(mDev, &isDataReady);
    if (isDataReady)
    {
        VL53L0X_GetRangingMeasurementData(mDev, &rangingData);
        if (rangingData.RangeStatus == 0)
        {
            mLastMeasurement = rangingData.RangeMilliMeter;
        }
        else
        {
            mLastMeasurement = INVALID_VALUE;
        }
    }
    else
    {
        mLastMeasurement = INVALID_VALUE;
    }

    return mLastMeasurement;
}

}  // namespace sensors