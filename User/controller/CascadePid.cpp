#include "CascadePid.hh"

namespace controller
{

CascadePid::CascadePid(double posP, double posI, double posD, double velP, double velI, double velD)
    : mPidPos{posP, posI, posD}
    , mPidVel{velP, velI, velD}
{
}

CascadePid::PidOut CascadePid::calculate(const PidIn& pidIn)
{
    static int posCnt = 0;
    // position part
    if (++posCnt == 10)
    {
        posCnt = 0;

        double errorPos = pidIn.refPos - pidIn.measPos;
        mPidPos.lastOut = mPidPos.kp * errorPos;
    }

    // velocity part
    double errorVel = mPidPos.lastOut + mPidVel.feedForward * pidIn.refVel - pidIn.measVel;
    double velOut   = mPidVel.kp * errorVel;
    // smart integral
    if (pidIn.refVel == 0)
    {
        mPidVel.errorSum += mPidVel.ki * errorVel;
        velOut += mPidVel.errorSum;
    }

    PidOut out;
    out.left = out.right = velOut;
    return out;
}

}  // namespace controller