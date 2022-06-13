#pragma once

#include "controller/Command.hh"
#include "controller/CommandQueue.hh"
#include "mycha/MouseData.hh"
#include "utils/AllSignals.hh"
#include "utils/LoggingSystem.hh"
#include "utils/Stack.hh"
#include <cstdint>
#include <utility>

namespace logic
{
constexpr uint8_t mazeLen = 16;

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

bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);

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
    void drawMazeWeights();
    void drawMazeWalls();
    void logMouseAndMaze(utils::LoggingSystem& logger);
    bool isMouseInFinishPoint();
    bool isMouseInStartingPoint();
    controller::Command generateCommandInSearchRun() const;
    void generateCommandsToBackToStart(controller::CommandQueue& queue);

  private:
    void moveMouseForward();
    void markConnectionBetweenCells(const Point& p1, const Point& p2);
    void rotateMouse(controller::RotationDir dir);
    void updateWalls(const mycha::DistancesData& distances);

    // they updates wall in cell and wall in adjacent cell
    void updateWallN(const Point& point);
    void updateWallW(const Point& point);
    void updateWallS(const Point& point);
    void updateWallE(const Point& point);
    bool isConnectionBetweenCells(const Point& p1, const Point& p2);

    void initFloodfill();
    void floodfill(Point floodP);
    uint8_t minValueFromNeighbours(Point p);
    void pushToStackAdjacentCells(const Point& p, StackType& stack);

    Point findCellWithMinValueFromNeighbour() const;
    controller::Command getCommandFromCurrentPointToNext(Point next) const;

    utils::AllSignals& mSignals;

    const Point mStart;
    const Point mFinish;
    const MouseDir mMouseInitDir;
    Point mCurrentPos;
    MouseDir mMouseDir;
    Cell mMaze[mazeLen][mazeLen];
    // prevents errors when mouse pass through cells and then sensor perceives wall in this place
    // so it's not normal
    // used to help marking walls. So when first mouse passes through two cells,
    // and then it sees there walls, wall won't be inserted.
    // coordination same as in maze, but represents present of transitions between two cells
    // for maze 5x5, cells follows patter
    // 1 2 3 4 5
    // 6 7 8 9 10
    // ..........
    // ........25
    // when cells 6 and 7 are connected, then:
    // mIsCellsConnected[5][6] = true;
    // mIsCellsConnected[6][5] = true;
    // since cells start from 0 in array
    // bool mIsCellsConnected[mazeLen * mazeLen][mazeLen * mazeLen];
};

}  // namespace logic