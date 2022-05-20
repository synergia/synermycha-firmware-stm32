#include "Timer.hh"
#include "tim.h"

namespace utils
{

void Timer::start()
{
    HAL_TIM_Base_Start(&htim5);
}

void Timer::stop()
{
    HAL_TIM_Base_Stop(&htim5);
    mLastValueInUs = TIM5->CNT;
}

void Timer::reset()
{
    mLastValueInUs = 0;
    TIM5->CNT      = 0;
}

uint32_t Timer::getTimeInUs()
{
    return mLastValueInUs;
}

uint32_t Timer::getTimeInMs()
{
    return mLastValueInUs / 1000;
}

}  // namespace utils