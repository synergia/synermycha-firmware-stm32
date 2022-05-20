#pragma once

#include "controller/Command.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"
#include "utils/Stack.hh"
#include <cstdint>
#include <utility>

namespace logic
{
constexpr uint8_t mazeLen = 5;

/*
          X
 0------------------>
 |
 |
Y|
 |
 |
 V
*/
struct Point
{
    // column
    int8_t x{};
    // row
    int8_t y{};
};

using StackType = utils::Stack<Point, 256>;

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
    bool wasVisited{};
    uint8_t value = 255;
};

class Maze
{
  public:
    Maze(utils::AllSignals& signals);
    void updateMouseAndMazeState(const controller::Command& cmd, const mycha::DistancesData& distances);
    void updateWallsAtStartup(const mycha::DistancesData& distances);
    void drawMaze();

  private:
    void moveMouseForward();
    void rotateMouse(controller::RotationDir dir);
    void updateWalls(const mycha::DistancesData& distances);

    // they updates wall in cell and wall in adjacent cell
    void updateWallN(const Point& point);
    void updateWallW(const Point& point);
    void updateWallS(const Point& point);
    void updateWallE(const Point& point);

    void initFloodfill();
    void floodfill(Point floodP);
    uint8_t minValueFromNeighbours(Point p);
    void pushToStackAdjacentCells(const Point& p, StackType& stack);

    Point mStart;
    Point mFinish;
    Point mCurrentPos;
    Cell mMaze[mazeLen][mazeLen];
    MouseDir mMouseDir;
};

}  // namespace logic