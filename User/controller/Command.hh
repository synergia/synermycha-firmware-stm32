#pragma once

#include <cstdint>

namespace controller
{

enum class RotationDir : uint8_t
{
    Other,
    Left,
    Right,
    TurnBack
};

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
        , rotDir{RotationDir::Other}
    {
    }

    explicit RotationalCommand(RotationDir dir)
        : angle{rotDirToAngle(dir)}
        , rotDir(dir)

    {
    }
    // positive is rotating left
    double angle{};
    RotationDir rotDir{};

  private:
    double rotDirToAngle(RotationDir dir)
    {
        if (dir == RotationDir::Right)
            return -90.0;
        else if (dir == RotationDir::Left)
            return 90.0;
        else if (dir == RotationDir::TurnBack)
            return 180.0;
        else
            return 0.0;
    }
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