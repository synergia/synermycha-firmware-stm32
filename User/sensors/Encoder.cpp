#include "Encoder.hh"

namespace sensors
{

constexpr uint32_t defaultValue = 10000;

Encoder::Encoder(TimHandler* handler)
    : mHandler(handler)
{
}

void Encoder::initialize()
{
    HAL_TIM_Encoder_Start(mHandler, TIM_CHANNEL_ALL);
    mHandler->Instance->CNT = defaultValue;
}

int32_t Encoder::getDiffTicks() const
{
    // notice that when mouse is moving forward, CNT is decreasing
    int32_t tmp             = static_cast<int32_t>(mHandler->Instance->CNT);
    mHandler->Instance->CNT = defaultValue;
    return static_cast<int32_t>(defaultValue) - tmp;
}

}  // namespace sensors