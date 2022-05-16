#include "Encoder.hh"
#include <limits>

namespace sensors
{

// to minimize cnt loss, reset it only when close to border
// in one run, more than 2000 diff ticks is impossibble
// so take as minimum 6000 = 2000 + 4000 when mouse is rotating
// as maximum take max uint16_t - 2000
constexpr uint32_t defaultEncoderValue = 10000;
constexpr uint32_t minSafeCnt          = 6000;
constexpr uint32_t maxSafeCnt          = std::numeric_limits<uint16_t>::max() - 2000;

Encoder::Encoder(TimHandler* handler)
    : mHandler(handler)
    , mLastCnt{defaultEncoderValue}
    , isInitialized{false}
{
}

void Encoder::initialize()
{
    HAL_TIM_Encoder_Start(mHandler, TIM_CHANNEL_ALL);
    mHandler->Instance->CNT = defaultEncoderValue;
    isInitialized           = true;
}

int32_t Encoder::getDiffTicks()
{
    if (not isInitialized)
        return 0;

    // notice that when mouse is moving forward, CNT is decreasing
    int32_t tmpCnt = static_cast<int32_t>(mHandler->Instance->CNT);
    int32_t diff   = static_cast<int32_t>(mLastCnt) - tmpCnt;

    // avoid to much cnt loss
    if (tmpCnt > maxSafeCnt || tmpCnt < minSafeCnt)
    {
        mHandler->Instance->CNT = defaultEncoderValue;
        mLastCnt                = defaultEncoderValue;
    }
    else
    {
        mLastCnt = tmpCnt;
    }

    return diff;
}

}  // namespace sensors