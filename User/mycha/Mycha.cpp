#include "Mycha.hh"
#include <adc.h>
#include <cmath>
#include <inttypes.h>
#include <tim.h>

namespace mycha
{

Mycha::Mycha(utils::AllSignals& signals)
    : mSignals(signals)
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
    , mImu(sensors::address::imu)
{
    connectSignals();

    initializeMycha();
    display::clearDisplayBuff();
}

void Mycha::connectSignals()
{
    mSignals.interruptDistance.connect<Mycha, &Mycha::onInterruptDistance>(*this);
    mSignals.interruptController.connect<Mycha, &Mycha::onInterruptController>(*this);

    mSignals.buttonDown.connect<Mycha, &Mycha::onButtonDown>(*this);
    mSignals.buttonUp.connect<Mycha, &Mycha::onButtonUp>(*this);
    mSignals.tim14Elapsed.connect<Mycha, &Mycha::onTim14Elapsed>(*this);

    mSignals.getGyroZ.connect<Mycha, &Mycha::onGetGyroZ>(*this);
}

void Mycha::initializeMycha()
{
    initBle();
    initLed();
    initAdc();
    initMotors();
    initDistance();
    initImu();
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

void Mycha::initImu()
{
    mImu.initialize();
}

void Mycha::onInterruptDistance()
{
}

void Mycha::onInterruptController()
{
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
        mLed1.toggle();
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

    // volatile const auto Rticks             = mEncoderR.getDiffTicks();
    // volatile double rightWheelAngularSpeed = Rticks * mechanic::ticksToAngularSpeedMultipler;
    // volatile double rightWheelTransSpeed   = rightWheelAngularSpeed * mechanic::wheelRadius;

    // volatile const auto Lticks   = mEncoderL.getDiffTicks();
    // double leftWheelAngularSpeed = Lticks * mechanic::ticksToAngularSpeedMultipler;
    // double leftWheelTransSpeed   = leftWheelAngularSpeed * mechanic::wheelRadius;

    // double mouseTransSpeed   = (rightWheelTransSpeed + leftWheelTransSpeed) / 2;
    // double mouseAngularSpeed = (rightWheelTransSpeed - leftWheelTransSpeed) / mechanic::mouseRadius;

    // mSignals.displayLogValue.emit("V_L:%f", leftWheelTransSpeed, 0, false);
    // mSignals.displayLogValue.emit("V_R:%f", rightWheelTransSpeed, 1, false);

    // DrivingData data{};
    // data.angularSpeed = mouseAngularSpeed;
    // data.transSpeed   = mouseTransSpeed;
    // return data;
}

void Mycha::onButtonUp()
{
}

void Mycha::onButtonDown()
{
}

void Mycha::onTim14Elapsed()
{
    mLed3.toggle();

    DistancesData data;
    data.left       = mSensorL.readDistance();
    data.frontLeft  = mSensorFL.readDistance();
    data.front      = mSensorF.readDistance();
    data.frontRight = mSensorFR.readDistance();
    data.right      = mSensorR.readDistance();

    mSignals.setDistancesData.emit(data);

    // static bool colour = 0;
    // uint16_t PomiarADC = 0;
    // if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    // {
    //     PomiarADC = HAL_ADC_GetValue(&hadc1);
    //     HAL_ADC_Start(&hadc1);
    // }
    // SSD1306_DrawFilledRectangle(112,2,12,4,SSD1306_COLOR_BLACK);
    // if(PomiarADC/340 > 0){
    //   SSD1306_DrawRectangle(110,0,16,8,SSD1306_COLOR_WHITE);
    //   SSD1306_DrawLine(127,3,127,5,SSD1306_COLOR_WHITE);
    //   SSD1306_DrawFilledRectangle(112,2,PomiarADC/340,4,SSD1306_COLOR_WHITE);
    // }
    // else
    // {
    //   SSD1306_DrawRectangle(110,0,16,8,colour ? SSD1306_COLOR_WHITE : SSD1306_COLOR_BLACK );
    //   SSD1306_DrawLine(127,3,127,5,colour ? SSD1306_COLOR_WHITE : SSD1306_COLOR_BLACK);
    //   colour = !colour;
    // }
}

void Mycha::onGetGyroZ(double& gyroZ)
{
    gyroZ = mImu.getGyroZ();
}

}  // namespace mycha