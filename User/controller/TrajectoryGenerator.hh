#pragma once

#include <stdint.h>

namespace controller
{

class TrajectoryGenerator
{
  public:
    TrajectoryGenerator(double _xFinish, double _vMax, double _accel, double _dt);
    TrajectoryGenerator();

    void reset();

    void calculateTrajectory(uint16_t t);
    double getSRef() const;
    double getVRef() const;
    double getPathLength() const;

    double getXFinish() const;
    double getVMax() const;
    double getAccel() const;
    double getDt() const;

  private:
    // length of path in milimeters
    double xFinish;

    // max velocity in milimeters per second
    double vMax;

    // acceleration in milimiters per second square
    double accel;

    // period of controller (to get calculus from velocity)
    double dt;

    // reference distance
    double sRef = 0.0;

    // reference velocity
    double vRef = 0.0;

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