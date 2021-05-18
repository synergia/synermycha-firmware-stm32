#pragma once

namespace controller
{

/*
This controller uses two PID connected to each other.
Outer loop is position controller, inner loop velocity.
Velocity controller is run 10 times faster that position controller.

For now maybe we should use only P position, and PI velocity controller.

Integral part of velocity controller is "smart" -
it's equal zero for non-zero desired velocity.
*/
class CascadePid
{
  public:
    CascadePid(double posP, double posI, double posD, double velP, double velI, double velD);

    struct PidIn
    {
        double refPos;
        double measPos;

        double refVel;
        double measVel;
    };

    struct PidOut
    {
        double left;
        double right;
    };

    PidOut calculate(const PidIn& pidIn);

    struct Pid
    {
        double kp;
        double ki;
        double kd;
        double lastError;
        double errorSum;
        double lastOut;
        double feedForward;
    };

  private:
    Pid mPidPos;
    Pid mPidVel;
};

}  // namespace controller