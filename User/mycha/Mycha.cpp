#include "Mycha.hh"
#include "controller/Pid.hh"
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
    , mTrajectory(1, 0.6, 0.6)  // testing only
{
    connectSignals();

    initializeMycha();
    display::clearDisplayBuff();
}

void Mycha::initializeMycha()
{
    initBle();
    initLed();
    initAdc();
    initMotors();
    initDistance();
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

void Mycha::connectSignals()
{
    mSignals.interruptDistance.connect<Mycha, &Mycha::onInterruptDistance>(*this);
    mSignals.interruptController.connect<Mycha, &Mycha::onInterruptController>(*this);

    mSignals.buttonDown.connect<Mycha, &Mycha::buttonDown>(*this);
    mSignals.buttonUp.connect<Mycha, &Mycha::buttonUp>(*this);
    mSignals.tim14Elapsed.connect<Mycha, &Mycha::tim14Elapsed>(*this);
}

void Mycha::onInterruptDistance()
{
}

void Mycha::onInterruptController()
{
    static controller::Pid transSpeedPid{100, 5, 0};
    static controller::Pid angularSpeedPid{10, 0, 0};
    static controller::PidIn angSpeedPidIn, transSpeedPidIn;
    angSpeedPidIn.refVal   = 0;
    transSpeedPidIn.refVal = 0.4;

    const MouseData data    = getMouseData();
    angSpeedPidIn.measVal   = data.angularSpeed;
    transSpeedPidIn.measVal = data.transSpeed;
    double transSpeedOut    = transSpeedPid.calculate(transSpeedPidIn);
    double angSpeedOut      = angularSpeedPid.calculate(angSpeedPidIn);

    // mMotorL.setPwm(transSpeedOut - angSpeedOut);
    // mMotorR.setPwm(transSpeedOut + angSpeedOut);
    mMotorL.setPwm(0);
    mMotorR.setPwm(0);
}

MouseData Mycha::getMouseData()
{
    static int i = 0;
    if (++i == 200)
    {
        mLed1.toggle();
        i = 0;
    }

    volatile const auto Rticks             = mEncoderR.getDiffTicks();
    volatile double rightWheelAngularSpeed = Rticks * mechanic::ticksToAngularSpeedMultipler;
    volatile double rightWheelTransSpeed   = rightWheelAngularSpeed * mechanic::wheelRadius;

    volatile const auto Lticks   = mEncoderL.getDiffTicks();
    double leftWheelAngularSpeed = Lticks * mechanic::ticksToAngularSpeedMultipler;
    double leftWheelTransSpeed   = leftWheelAngularSpeed * mechanic::wheelRadius;

    double mouseTransSpeed   = (rightWheelTransSpeed + leftWheelTransSpeed) / 2;
    double mouseAngularSpeed = (rightWheelTransSpeed - leftWheelTransSpeed) / mechanic::mouseRadius;

    MouseData data{};
    data.angularSpeed = mouseAngularSpeed;
    data.transSpeed   = mouseTransSpeed;
    return data;
}

void Mycha::buttonUp()
{
}

void Mycha::buttonDown()
{
}

void Mycha::tim14Elapsed()
{
    mLed3.toggle();
    double distL  = mSensorL.readDistance();
    double distFL = mSensorFL.readDistance();
    double distF  = mSensorF.readDistance();
    mSignals.displayLogValue.emit("L=%f", distL, 0, false);
    mSignals.displayLogValue.emit("FL=%f", distFL, 1, false);
    mSignals.displayLogValue.emit("F=%f", distF, 2, true);
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

}  // namespace mycha