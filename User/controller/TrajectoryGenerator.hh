#pragma once

#include <stdint.h>

namespace controller
{

class TrajectoryGenerator
{
  public:
    TrajectoryGenerator(double _xFinish, double _vMax, double _accel);

    void setVMax(double _vMax)
    {
        vMax = _vMax;
    }

    void calculateTrajectory(uint16_t t);
    double getSRef() const;
    double getVRef() const;

  private:
    // length of path in milimeters
    double const xFinish;

    // max velocity in milimeters per second
    double vMax;

    // acceleration in milimiters per second square
    const double accel;

    // reference distance
    double sRef;

    // reference velocity
    double vRef;

    /*
     * trapezoidal profiler
     * profiler_a - lower left point
     * profiler_b - upper left point
     * profiler_c - upper right point
     * profiler_d - lower right point
     */
    double profiler_a;
    double profiler_b;
    double profiler_c;
    double profiler_d;
};

}  // namespace controller