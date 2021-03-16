#include "EventHandler.hh"
#include <main.h>

// used ONLY for calling controll loop directly from interrupt
// assigned in event handler
static utils::AllSignals* allSignalsPtr = nullptr;

// static flags to indicate that interrupt is needed to be handled
static bool flagButtonEnterPush;
static bool flagButtonDownPush;
static bool flagButtonUpPush;
static bool flagTim14Elapsed;

//****************************************************************
// INTERRUPT HANDLERS
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_OK_Pin)
    {
        if (HAL_GPIO_ReadPin(BUTTON_OK_GPIO_Port, BUTTON_OK_Pin) == GPIO_PIN_RESET)
            flagButtonEnterPush = true;
    }
    if (GPIO_Pin == BUTTON_DOWN_Pin)
    {
        if (HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_RESET)
            flagButtonDownPush = true;
    }
    if (GPIO_Pin == BUTTON_UP_Pin)
    {
        if (HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_RESET)
            flagButtonUpPush = true;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    // if TIM9 is used as controll loop, it should looks like follow
    if (htim->Instance == TIM10)
    {
        if (allSignalsPtr)
            allSignalsPtr->interruptController.emit();
    }

    // rest of timers
    if (htim->Instance == TIM14)
    {
        flagTim14Elapsed = true;
    }
}
//**************************************************************

//**************************************************************
// EVENT HANDLER
EventHandler::EventHandler(utils::AllSignals& signals)
    : mSignals(signals)
{
    // assigning to static global variable address to make possible
    // to call signal directly from interrupt
    allSignalsPtr = &mSignals;
}

void EventHandler::HandleEvents()
{
    while (true)
    {
        if (flagButtonEnterPush)
        {
            mSignals.buttonEnter.emit();
            flagButtonEnterPush = false;
        }
        if (flagButtonDownPush)
        {
            mSignals.buttonDown.emit();
            flagButtonDownPush = false;
        }
        if (flagButtonUpPush)
        {
            mSignals.buttonUp.emit();
            flagButtonUpPush = false;
        }
        if (flagTim14Elapsed)
        {
            mSignals.tim14Elapsed.emit();
            flagTim14Elapsed = false;
        }
    }
}