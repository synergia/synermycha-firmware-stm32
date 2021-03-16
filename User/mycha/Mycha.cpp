#include "Mycha.hh"
#include <adc.h>
#include <tim.h>

Mycha::Mycha(utils::AllSignals& signals)
    : mSignals(signals)
    , mSensorL(VC53L0x_XSHUT_LEFT_GPIO_Port, VC53L0x_XSHUT_LEFT_Pin, 0b0101010)
    , mSensorFL(VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port, VC53L0x_XSHUT_FRONT_LEFT_Pin, 0b0101011)
    , mSensorF(VC53L0x_XSHUT_FRONT_GPIO_Port, VC53L0x_XSHUT_FRONT_Pin, 0b0101100)
    , mSensorFR(VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port, VC53L0x_XSHUT_FRONT_RIGHT_Pin, 0b0101101)
    , mSensorR(VC53L0x_XSHUT_RIGHT_GPIO_Port, VC53L0x_XSHUT_RIGHT_Pin, 0b0101111)
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
{
    connectSignals();

    initializeMycha();
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
    constexpr float distanceToReach = 1;  // 1 meter
    static float currentDistance    = 0;
    static uint32_t lastTickL;
    static uint32_t lastTickR;
}

void Mycha::buttonUp()
{
    mLed1.toggle();
}

void Mycha::buttonDown()
{
    mLed2.toggle();
}

void Mycha::tim14Elapsed()
{
    static bool colour = 0;
    uint16_t PomiarADC = 0;
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        PomiarADC = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Start(&hadc1);
    }
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