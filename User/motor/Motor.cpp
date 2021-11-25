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
    , isForward(true)
{
}

void Motor::initialize()
{
    HAL_TIM_PWM_Start(mHandler, mChannel);
    setDir(true);
    *mCcr = 0;
}

void Motor::setPwm(float speed)
{
    if (speed > MAX_PWM)
        speed = MAX_PWM;
    else if (speed < -MAX_PWM)
        speed = -MAX_PWM;

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