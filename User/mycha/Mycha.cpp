#include "Mycha.hh"
#include "utils/Timer.hh"
#include <adc.h>
#include <cmath>
#include <inttypes.h>
#include <tim.h>

namespace
{
/// @note lack of front left/rigth, for tests
/// when they are present, it seemed to be 0
/// need check it
inline bool areAllDistancesInitialized(const mycha::DistancesData& dist)
{
    return dist.right != 0 and dist.front != 0 and dist.left != 0;
}

}  // namespace

namespace mycha
{

Mycha::Mycha(utils::AllSignals& signals, utils::LoggingSystem& logger)
    : mSignals(signals)
    , mLogger{logger}
    , mSensorL(VC53L0x_XSHUT_LEFT_GPIO_Port, VC53L0x_XSHUT_LEFT_Pin, sensors::address::left,
               sensors::calibration::offsetLeft)
    , mSensorFL(VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port, VC53L0x_XSHUT_FRONT_LEFT_Pin, sensors::address::frontLeft,
                sensors::calibration::outFrontLeft)
    , mSensorF(VC53L0x_XSHUT_FRONT_GPIO_Port, VC53L0x_XSHUT_FRONT_Pin, sensors::address::front,
               sensors::calibration::offsetFront)
    , mSensorFR(VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port, VC53L0x_XSHUT_FRONT_RIGHT_Pin, sensors::address::frontRight,
                sensors::calibration::offsetFrontRight)
    , mSensorR(VC53L0x_XSHUT_RIGHT_GPIO_Port, VC53L0x_XSHUT_RIGHT_Pin, sensors::address::right,
               sensors::calibration::offsetRight)
    , mLed1(signals, LED_1_GPIO_Port, LED_1_Pin)
    , mLed2(signals, LED_2_GPIO_Port, LED_2_Pin)
    , mLed3(signals, LED_3_GPIO_Port, LED_3_Pin)
    , mLed4(signals, LED_4_GPIO_Port, LED_4_Pin)
    , mLed5(signals, LED_5_GPIO_Port, LED_5_Pin)
    , mHBridge(DRV8835_EN_GPIO_Port, DRV8835_EN_Pin)
    , mMotorL(&htim12, &(htim12.Instance->CCR2), TIM_CHANNEL_2, DRV8835_DIR_B_GPIO_Port, DRV8835_DIR_B_Pin, false)
    , mMotorR(&htim12, &(htim12.Instance->CCR1), TIM_CHANNEL_1, DRV8835_DIR_A_GPIO_Port, DRV8835_DIR_A_Pin, true)
    , mEncoderL(&htim3)
    , mEncoderR(&htim4)
    , mImu(signals, sensors::address::imu)
{
    display::clearDisplayBuff();
    initializeMycha();
    HAL_Delay(1000);
    display::clearDisplayBuff();

    /// @note important thing, that connectSignal should be after initalization of mycha
    connectSignals();
    mLogger.info("Mycha is ready");
}

void Mycha::connectSignals()
{
    mSignals.interruptDistance.connect<Mycha, &Mycha::onInterruptDistance>(*this);
    mSignals.interruptController.connect<Mycha, &Mycha::onInterruptController>(*this);

    mSignals.buttonDown.connect<Mycha, &Mycha::onButtonDown>(*this);
    mSignals.buttonUp.connect<Mycha, &Mycha::onButtonUp>(*this);
    mSignals.buttonEnter.connect<Mycha, &Mycha::onButtonEnter>(*this);
    mSignals.setLed1.connect<Mycha, &Mycha::onSetLed1>(*this);
    mSignals.setLed2.connect<Mycha, &Mycha::onSetLed2>(*this);
    mSignals.setLed3.connect<Mycha, &Mycha::onSetLed3>(*this);
    mSignals.setLed4.connect<Mycha, &Mycha::onSetLed4>(*this);
    mSignals.tim14Elapsed.connect<Mycha, &Mycha::onTim14Elapsed>(*this);

    mSignals.getGyroZ.connect<Mycha, &Mycha::onGetGyroZ>(*this);
}

void Mycha::initializeMycha()
{
    char buf[20];

    // initBle();
    initLed();
    initAdc();
    snprintf(buf, 20, "Voltage=%f", getBatteryVoltage());
    mSignals.displayTextLine.emit(-1, buf);

    initMotors();
    initDistance();
    mSignals.displayTextLine.emit(1, "ToF - OK");

    auto offset = initImu();
    snprintf(buf, 20, "offset gyro=%d", offset);
    mSignals.displayTextLine.emit(2, buf);
}

void Mycha::initBle()
{
}

void Mycha::initLed()
{
    mLed1.initialize();
    mLed2.initialize();
    mLed3.initialize();
    mLed4.initialize();
    mLed5.initialize();
}

void Mycha::initAdc()
{
    HAL_ADC_Start(&hadc1);
}

void Mycha::initMotors()
{
    mHBridge.initialize();

    mMotorL.initialize();
    mMotorR.initialize();

    mEncoderL.initialize();
    mEncoderR.initialize();
}

void Mycha::initDistance()
{
    mSensorL.initialize();
    mSensorFL.initialize();
    mSensorF.initialize();
    mSensorFR.initialize();
    mSensorR.initialize();
}

int16_t Mycha::initImu()
{
    return mImu.initialize();
}

void Mycha::onInterruptDistance()
{
}

void Mycha::onInterruptController()
{
    if (not isMychaInitalized())
        return;

    setDrivingData();

    MotorsSettings motorSettings;
    mSignals.getMotorSettings.emit(motorSettings);

    // mSignals.displayLogValue.emit("motL:%f", (double)motorSettings.pwmLeftMotor, 2, false);
    // mSignals.displayLogValue.emit("motR:%f", (double)motorSettings.pwmRightMotor, 3, true);

    mMotorL.setPwm(motorSettings.pwmLeftMotor);
    mMotorR.setPwm(motorSettings.pwmRightMotor);

    // mMotorL.setPwm(0);
    // mMotorR.setPwm(0);
}

void Mycha::setDrivingData()
{
    static int i = 0;
    if (++i == 100)
    {
        // mLed1.toggle();
        i = 0;
    }

    const auto Rticks          = mEncoderR.getDiffTicks();
    const auto Lticks          = mEncoderL.getDiffTicks();
    const double rightDistance = Rticks * mechanic::ticksToDistanceMultipler;
    const double leftDistance  = Lticks * mechanic::ticksToDistanceMultipler;

    DrivingData data{};
    data.leftWheelDistance  = leftDistance;
    data.rightWheelDistance = rightDistance;
    mSignals.setDrivingData.emit(data);
}

void Mycha::onButtonUp()
{
}

void Mycha::onButtonDown()
{
}

void Mycha::onButtonEnter()
{
}

void Mycha::onSetLed1(bool isOn)
{
    mLed1.set(isOn);
}

void Mycha::onSetLed2(bool isOn)
{
    mLed2.set(isOn);
}

void Mycha::onSetLed3(bool isOn)
{
    mLed3.set(isOn);
}

void Mycha::onSetLed4(bool isOn)
{
    mLed4.set(isOn);
}

void Mycha::onTim14Elapsed()
{
    // mLed3.toggle();

    DistancesData data;
    data.left       = mSensorL.readDistance();
    data.frontLeft  = mSensorFL.readDistance();
    data.front      = mSensorF.readDistance();
    data.frontRight = mSensorFR.readDistance();
    data.right      = mSensorR.readDistance();

    mAreDistancesValidData = areAllDistancesInitialized(data);

    if (mAreDistancesValidData)
        mSignals.setDistancesData.emit(data);
}

void Mycha::onGetGyroZ(double& gyroZ)
{
    gyroZ = mImu.getGyroZ();
}

double Mycha::getBatteryVoltage()
{
    double voltage{};
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        auto rawAdc = HAL_ADC_GetValue(&hadc1);
        // resistance division is 3
        // reference voltage 3.3V
        voltage = 3.0 * 3.3 * (double(rawAdc) / 4096.0);
        HAL_ADC_Start(&hadc1);
    }
    return voltage;
}

bool Mycha::isMychaInitalized()
{
    return mAreDistancesValidData;
}

}  // namespace mycha