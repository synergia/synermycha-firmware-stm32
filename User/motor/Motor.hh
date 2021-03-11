#pragma once

#include "utils/HalUtils.hh"

// pwm in range [-MAX_PWM_RANGE ... +MAX_PWM_RANGE]
constexpr float MAX_PWM_RANGE = 100.0f;

class Motor
{
  public:
    Motor(TimHandler* handler, PwmCcrPtr ccr, uint32_t channel, GpioPort portDir, GpioPin pinDir,
          bool pinStateWhenForward);

    // [-MAX_PWM_RANGE ... +MAX_PWM_RANGE]
    void setPwm(float pwm);

  private:
    void setDir(bool forward);

    TimHandler* mHandler;
    PwmCcrPtr mCcr;
    uint32_t mMaxCcr;
    uint32_t mChannel;
    GpioPort mPortDir;
    GpioPin mPinDir;
    const bool mIsPinHighWhenForward;

    // current pwm is in range 0...100
    // factor is used to calculate range 0...mMaxCcr to 0...100
    float mFactor;
    uint32_t mCurrentPwm;

    bool isForward;
};