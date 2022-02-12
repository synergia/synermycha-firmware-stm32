#pragma once

#include <cstdint>

namespace controller
{

struct ForwardCommand
{
    ForwardCommand() = default;
    explicit ForwardCommand(uint8_t cubes)
        : nrOfCubes(cubes)
    {
    }
    // nr of cubes in labirynth to go through
    uint8_t nrOfCubes = 0;
};

struct RotationalCommand
{
    RotationalCommand() = default;
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