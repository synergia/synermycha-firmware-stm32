#include "Motor.hh"
#include <algorithm>

Motor::Motor(TimHandler* handler, PwmCcrPtr ccr, uint32_t channel, GpioPort portDir, GpioPin pinDir,
             bool pinStateWhenForward)
    : mHandler(handler)
    , mCcr(ccr)
    , mMaxCcr(handler->Init.Period)
    , mChannel(channel)
    , mPortDir(portDir)
    , mPinDir(pinDir)
    , mIsPinHighWhenForward(pinStateWhenForward)
    , mFactor(mMaxCcr / MAX_PWM_RANGE)
    , mCurrentPwm(0)
    , isForward(true)
{
    GpioWrite(mPortDir, mPinDir, mIsPinHighWhenForward);
}

void Motor::setPwm(float speed)
{
    if (speed > MAX_PWM_RANGE)
        return;

    mCurrentPwm = speed;

    uint32_t valueToCcr = static_cast<uint32_t>(std::abs(mFactor * speed));
    if (speed >= 0)
    {
        setDir(true);
    }
    else
    {
        setDir(false);
    }
    *mCcr = valueToCcr;
}

void Motor::setDir(bool forward)
{
    isForward = forward;
    if (forward)
    {
        GpioWrite(mPortDir, mPinDir, mIsPinHighWhenForward);
    }
    else
    {
        GpioWrite(mPortDir, mPinDir, not mIsPinHighWhenForward);
    }
}