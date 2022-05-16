#pragma once

#include <cstdint>

namespace controller
{

struct ForwardCommand
{
    ForwardCommand() = default;
    // in meters
    explicit ForwardCommand(double len)
        : length(len)
    {
    }
    // nr of cubes in labirynth to go through
    double length = 0;
};

struct RotationalCommand
{
    RotationalCommand() = default;
    // in degrees
    explicit RotationalCommand(double ang)
        : angle(ang)
    {
    }
    // positive is rotating left
    double angle = 0;
};

enum class CommandType
{
    Forward,
    Rotational
};

struct Command
{
    Command() = default;
    CommandType type;
    // dependign on the type, only one value is correct
    ForwardCommand forward;
    RotationalCommand rotational;
};

}  // namespace controller