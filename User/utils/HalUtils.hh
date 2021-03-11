#pragma once

#include <main.h>

using GpioPort   = GPIO_TypeDef*;
using GpioPin    = uint16_t;
using TimHandler = TIM_HandleTypeDef;
using PwmCcrPtr  = volatile uint32_t*;

static inline void GpioWrite(GpioPort port, GpioPin pin, bool isSet)
{
    HAL_GPIO_WritePin(port, pin, static_cast<GPIO_PinState>(isSet));
}

static inline bool GpioRead(GpioPort port, GpioPin pin)
{
    return static_cast<bool>(HAL_GPIO_ReadPin(port, pin));
}

static inline void WaitMs(uint32_t delay)
{
    HAL_Delay(delay);
}