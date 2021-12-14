#pragma once
#include "utils/AllSignals.hh"
#include "utils/HalUtils.hh"
#include <i2c.h>

namespace sensors
{

namespace address
{
constexpr uint8_t imu = 0b1101000;
}

namespace registers
{
constexpr uint8_t GYRO_CONFIG{0x1B};
constexpr uint8_t GYRO_ZOUT_H{0x47};
constexpr uint8_t GYRO_ZOUT_L{0x48};
constexpr uint8_t WHO_AM_I{0x75};
}  // namespace registers

namespace bitfields
{
constexpr uint8_t WHO_AM_I_ANSWER{0x71};
}  // namespace bitfields

enum class GyroDps : uint8_t
{
    GYRO_FS_SEL_250dps,
    GYRO_FS_SEL_500dps,
    GYRO_FS_SEL_1000dps,
    GYRO_FS_SEL_2000dps,
};

class Imu
{
  public:
    Imu(utils::AllSignals& signals, uint8_t address);
    void initialize();

    void setGyroDps(GyroDps gyroDps);

    double getGyroZ();
    uint16_t getWhoAmI();
    uint16_t getCurrentDpsValue();

  private:
    utils::AllSignals& mSignals;

    int16_t getRawGyroZ();

    I2cHandlerPtr mI2cHandler;
    uint8_t mAddress;
    uint16_t mCurrentDps;

    int16_t mOffset{};
};

}  // namespace sensors