#ifndef __RN847x_HPP__
#define __RN847x_HPP__

#define HANDLER_BATT           0072
#define HANDLER_BATT_VOLTAGE   0075
#define HANDLER_INTERNAL_STATE 0078
#define HANDLER_IMU_EULER      007B
#define HANDLER_DISTANCE_RAW   007E
#define HANDLER_ENCODER_TICS   0081
#define HANDLER_MOTOR_PWM      0084
#define HANDLER_MAP            0092
#define HANDLER_MODE_R_FI      00B2
#define HANDLER_RGB_LED        00B5
#define HANDLER_PID            00B8

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    class RN487x
    {
      public:
        RN487x();
        bool EnterCMD();
        bool SendCMD(char _command[]);
        bool ReadBLECharacteristic(char _handler[]);
        bool SetBLECharacteristic(char _handler[]);
        bool RestartAndExitCMD();
        void ExitCMD();

      private:
    };

#ifdef __cplusplus
}
#endif

#endif