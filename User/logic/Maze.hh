#pragma once

#include "controller/Command.hh"
#include "mycha/MouseData.hh"
#include <cstdint>
#include <utility>

namespace logic
{
constexpr uint8_t mazeLen = 5;

struct Point
{
    uint8_t x{};
    uint8_t y{};
};

enum class MouseDir : uint8_t
{
    N,
    S,
    E,
    W
};

struct Cell
{
    bool isWallN{};
    bool isWallS{};
    bool isWallE{};
    bool isWallW{};
};

class Maze
{
  public:
    Maze();
    void updateMouseAndMazeState(const controller::Command& cmd, const mycha::DistancesData& distances);
    void updateWallsAtStartup(const mycha::DistancesData& distances);

  private:
    void moveMouseForward();
    void rotateMouse(controller::RotationDir dir);
    void updateWalls(const mycha::DistancesData& distances);

    // they updates wall in cell and wall in adjacent cell
    void updateWallN(const Point& point);
    void updateWallW(const Point& point);
    void updateWallS(const Point& point);
    void updateWallE(const Point& point);

    Point mStart;
    Point mFinish;
    Point mCurrentPos;
    Cell mMaze[mazeLen][mazeLen];
    MouseDir mMouseDir;
};

}  // namespace logic