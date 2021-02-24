#include "RN4871.hpp"

extern UART_HandleTypeDef huart1;

bool RN487x::EnterCMD()
{
    HAL_Delay(100);
    uint8_t command[4] = "$$$";
    HAL_UART_Transmit_DMA(&huart1, command, 3);
    char response[5];
}

bool RN487x::SendCMD(char _command[])
{
    HAL_UART_Transmit(&huart1, (uint8_t*)_command, sizeof(_command) / sizeof(char), 10);
    char carriageReturn = '\r';
    HAL_UART_Transmit(&huart1, (uint8_t*)&carriageReturn, 1, 10);
}

bool RN487x::RestartAndExitCMD()
{
    char command[] = "R,1";
    SendCMD(command);
}

void RN487x::ExitCMD()
{
    char command[] = "---";
    SendCMD(command);
}