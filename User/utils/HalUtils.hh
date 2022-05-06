#pragma once

#include <cstdint>
#include <main.h>

using GpioPort             = GPIO_TypeDef*;
using GpioPin              = uint16_t;
using TimHandler           = TIM_HandleTypeDef;
using I2cHandlerPtr        = I2C_HandleTypeDef*;
using PwmCcrPtr            = volatile uint32_t*;
using LoggingWorkerPtrType = bool (*)(char* buf, int len);

static inline void gpioWrite(GpioPort port, GpioPin pin, bool isSet)
{
    HAL_GPIO_WritePin(port, pin, static_cast<GPIO_PinState>(isSet));
}

static inline bool gpioRead(GpioPort port, GpioPin pin)
{
    return static_cast<bool>(HAL_GPIO_ReadPin(port, pin));
}

static inline bool i2cWriteBytes(I2cHandlerPtr handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
{
    return static_cast<bool>(
        HAL_I2C_Mem_Write(handler, devAddr << 1, (uint16_t)regAddr, 1, data, (uint16_t)length, 100));
}

static inline bool i2cWriteOneByte(I2cHandlerPtr handler, uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return static_cast<bool>(i2cWriteBytes(handler, devAddr, regAddr, 1, &data));
}

static inline bool i2cReadBytes(I2cHandlerPtr handler, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
{
    return static_cast<bool>(
        HAL_I2C_Mem_Read(handler, devAddr << 1, (uint16_t)regAddr, 1, data, (uint16_t)length, 100));
}

static inline bool i2cReadOneByte(I2cHandlerPtr handler, uint8_t devAddr, uint8_t regAddr, uint8_t* data)
{
    return i2cReadBytes(handler, devAddr, regAddr, 1, data);
}

static inline void WaitMs(uint32_t delay)
{
    HAL_Delay(delay);
}