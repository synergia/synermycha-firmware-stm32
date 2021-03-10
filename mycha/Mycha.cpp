#include "Mycha.hh"

Mycha::Mycha(utils::AllSignals& signals)
    : mSignals(signals)
    , mSensorL(VC53L0x_XSHUT_LEFT_GPIO_Port, VC53L0x_XSHUT_LEFT_Pin, 0b0101010)
    , mSensorFL(VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port, VC53L0x_XSHUT_FRONT_LEFT_Pin, 0b0101011)
    , mSensorF(VC53L0x_XSHUT_FRONT_GPIO_Port, VC53L0x_XSHUT_FRONT_Pin, 0b0101100)
    , mSensorFR(VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port, VC53L0x_XSHUT_FRONT_RIGHT_Pin, 0b0101101)
    , mSensorR(VC53L0x_XSHUT_RIGHT_GPIO_Port, VC53L0x_XSHUT_RIGHT_Pin, 0b0101111)
{
    mSignals.interruptDistance.connect<Mycha, &Mycha::onInterruptDistance>(*this);
    mSignals.interruptControll.connect<Mycha, &Mycha::onInterruptControll>(*this);
}

void Mycha::onInterruptDistance()
{
}

void Mycha::onInterruptControll()
{
}