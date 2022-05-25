#include "Imu.hh"
#include <limits>

namespace sensors
{
namespace
{
uint8_t gyroDpsToBitfieldValue(GyroDps gyroDps)
{
    switch (gyroDps)
    {
    case GyroDps::GYRO_FS_SEL_250dps:
        return 0;
    case GyroDps::GYRO_FS_SEL_500dps:
        return 0x8;
    case GyroDps::GYRO_FS_SEL_1000dps:
        return 0x10;
    case GyroDps::GYRO_FS_SEL_2000dps:
        return 0x18;
    default:
        return 0x18;
    }
}

uint16_t gyroDpsToRawValue(GyroDps gyroDps)
{
    switch (gyroDps)
    {
    case GyroDps::GYRO_FS_SEL_250dps:
        return 250;
    case GyroDps::GYRO_FS_SEL_500dps:
        return 500;
    case GyroDps::GYRO_FS_SEL_1000dps:
        return 1000;
    case GyroDps::GYRO_FS_SEL_2000dps:
        return 2000;
    default:
        return 2000;
    }
}
}  // namespace

Imu::Imu(utils::AllSignals& signals, uint8_t address)
    : mSignals(signals)
    , mI2cHandler(&hi2c3)
    , mAddress(address)
    , mCurrentDps(0)
{
}

int16_t Imu::initialize()
{
    setGyroDps(GyroDps::GYRO_FS_SEL_1000dps);

    HAL_Delay(100);
    int32_t gyroSumZ = 0;
    for (int i = 0; i < 1000; i++)
    {
        gyroSumZ += getRawGyroZ();
        HAL_Delay(1);
    }
    mOffset = gyroSumZ / 1000.0;

    return mOffset;
}

double Imu::getGyroZ()
{
    constexpr auto maxAdc = std::numeric_limits<int16_t>::max();
    const auto rawGyro    = getRawGyroZ() - mOffset;
    const double value    = ((double)rawGyro / maxAdc) * mCurrentDps;
    return value;
}

int16_t Imu::getRawGyroZ()
{
    uint8_t zAxe[2];
    i2cReadBytes(mI2cHandler, mAddress, registers::GYRO_ZOUT_H, 2, zAxe);
    uint16_t ret = zAxe[0];
    ret <<= 8;
    ret |= zAxe[1];

    return ret;
}

uint16_t Imu::getWhoAmI()
{
    uint8_t whoAmI = 0;
    i2cReadOneByte(mI2cHandler, mAddress, registers::WHO_AM_I, &whoAmI);
    return whoAmI;
}

void Imu::setGyroDps(GyroDps gyroDps)
{
    i2cWriteOneByte(mI2cHandler, mAddress, registers::GYRO_CONFIG, gyroDpsToBitfieldValue(gyroDps));
    mCurrentDps = gyroDpsToRawValue(gyroDps);
}

uint16_t Imu::getCurrentDpsValue()
{
    return mCurrentDps;
}

}  // namespace sensors