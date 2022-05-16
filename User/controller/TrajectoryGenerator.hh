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

    void calculateTrajectory(double t);
    double getSRef() const;
    double getVRef() const;
    double getPathLength() const;

  private:
    // length of path in milimeters
    double xFinish;

    // max velocity in meters per second
    double vMax;

    // acceleration in meters per second square
    double accel;

    // period of controller (to get calculus from velocity)
    double dt;

    // reference distance
    double sRef = 0.0;

    // reference velocity
    double vRef = 0.0;
    // used in trapezoidal integration
    double lastVRef = 0.0;

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