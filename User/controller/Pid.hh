#pragma once

namespace controller
{

struct PidIn
{
    double refVal;
    double measVal;
};

class Pid
{
  public:
    Pid(double kp, double ki, double kd);

    double calculate(const PidIn& pidIn);
    void reset();

  private:
    double mKp;
    double mKi;
    double mKd;
    double mLastError = 0;
    double mErrorSum  = 0;
};

}  // namespace controller