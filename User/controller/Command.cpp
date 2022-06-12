#include "Command.hh"

namespace controller
{

const char* toCString(RotationDir dir)
{
    switch (dir)
    {
    case RotationDir::Left:
        return "left";
    case RotationDir::Right:
        return "right";
    case RotationDir::TurnBack:
        return "turnBack";
    case RotationDir::Other:
        return "other";
    default:
        return "unknown";
    }
}

const char* toCString(CommandType type)
{
    switch (type)
    {
    case CommandType::Forward:
        return "forward";
    case CommandType::Rotational:
        return "rotational";
    case CommandType::Stop:
        return "stop";
    default:
        return "unknown";
    }
}

}  // namespace controller