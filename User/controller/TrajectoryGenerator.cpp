#include "TrajectoryGenerator.hh"
#include <cmath>

namespace controller
{

TrajectoryGenerator::TrajectoryGenerator(double _xFinish, double _vMax, double _accel)
    : xFinish(_xFinish)
    , vMax(_vMax)
    , accel(_accel)
{
    sRef       = 0;
    vRef       = 0;
    profiler_a = 0;
    profiler_b = 0;
    profiler_c = 0;
    profiler_d = 0;

    double tMax = xFinish / vMax - vMax / accel;

    if (tMax < 0)
    {
        // trapezoidal profile cannot be created for given acceleration
        // so create triangle profiler
        vMax = std::sqrt(xFinish * accel);
        tMax = 0;
    }

    profiler_a = 0;
    profiler_b = vMax / accel;
    profiler_c = vMax / accel + tMax;
    profiler_d = vMax / accel + tMax + vMax / accel;
}

void TrajectoryGenerator::calculateTrajectory(uint16_t t)
{
    double y = 0;

    if (t <= profiler_a)
    {
        y = 0;
    }
    else if ((profiler_a <= t) && (t <= profiler_b))
    {
        y = (t - profiler_a) / (profiler_b - profiler_a);
    }
    else if ((profiler_b <= t) && (t <= profiler_c))
    {
        y = 1;
    }
    else if ((profiler_c <= t) && (t <= profiler_d))
    {
        y = (profiler_d - t) / (profiler_d - profiler_c);
    }
    else
    {
        y = 0;
    }

    vRef = y * vMax;

    // integrate velocity to get position
    sRef += vRef * 0.001;
}

double TrajectoryGenerator::getSRef() const
{
    return sRef;
}

double TrajectoryGenerator::getVRef() const
{
    return vRef;
}

}  // namespace controller