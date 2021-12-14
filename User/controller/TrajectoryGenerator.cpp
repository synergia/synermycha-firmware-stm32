#include "TrajectoryGenerator.hh"
#include <cmath>

namespace controller
{

TrajectoryGenerator::TrajectoryGenerator(double _xFinish, double _vMax, double _accel, double _dt)
    : xFinish(_xFinish)
    , vMax(_vMax)
    , accel(_accel)
    , dt(_dt)
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

TrajectoryGenerator::TrajectoryGenerator()
    : xFinish(0)
    , vMax(0)
    , accel(0)
    , dt(0)
{
}

void TrajectoryGenerator::reset()
{
    vRef     = 0;
    lastVRef = 0;
    sRef     = 0;
}

void TrajectoryGenerator::calculateTrajectory(double t)
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
    sRef += (lastVRef + vRef) * dt * 0.5;
    lastVRef = vRef;
}

double TrajectoryGenerator::getSRef() const
{
    return sRef;
}

double TrajectoryGenerator::getVRef() const
{
    return vRef;
}

double TrajectoryGenerator::getPathLength() const
{
    return xFinish;
}

}  // namespace controller