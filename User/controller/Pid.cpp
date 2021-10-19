#include "Pid.hh"

namespace controller
{

Pid::Pid(double kp, double ki, double kd)
    : mKp{kp}
    , mKi{ki}
    , mKd{kd}
{
}

double Pid::calculate(const PidIn& pidIn)
{
    double error = pidIn.refVal - pidIn.measVal;

    double p = error * mKp;

    mErrorSum += error;
    double i = mErrorSum * mKi;

    double d   = (error - mLastError) * mKd;
    mLastError = error;

    return p + i + d;
}

}  // namespace controller