#include "Encoder.hh"

namespace sensors
{

Encoder::Encoder(TimHandler* handler)
    : mHandler(handler)
{
}

void Encoder::initialize()
{
    HAL_TIM_Encoder_Start(mHandler, TIM_CHANNEL_ALL);
}

uint32_t Encoder::getTicks() const
{
    return mHandler->Instance->CNT;
}

}  // namespace sensors