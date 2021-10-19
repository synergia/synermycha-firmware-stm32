#pragma once

namespace controller
{

struct PidIn
{
    double refVal;
    double measVal;
};

/*
This controller uses two PID connected to each other.
Outer loop is position controller, inner loop velocity.
Velocity controller is run 10 times faster that position controller.

For now maybe we should use only P position, and PI velocity controller.

Integral part of velocity controller is "smart" -
it's equal zero for non-zero desired velocity.
*/
class Pid
{
  public:
    Pid(double kp, double ki, double kd);

    double calculate(const PidIn& pidIn);

  private:
    double mKp;
    double mKi;
    double mKd;
    double mLastError = 0;
    double mErrorSum  = 0;
};

}  // namespace controller