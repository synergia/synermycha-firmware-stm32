#include "Maze.hh"
#include "display/Display.hh"
#include "mycha/Mycha.hh"
#include "sensors/DistanceTof.hh"
#include "utils/Timer.hh"
#include <cstring>

namespace
{

logic::StackType stack;
utils::Stack<controller::Command, 1000> cmdStack;
static char bufMazeWalls[5][20];

bool cmpEqual(double v1, double v2)
{
    return std::abs(v1 - v2) < 1e-5;
}

// assume that char buf is always 4 size. max value is 255 + \0;
void valueToCharBuf(uint8_t val, char* buf)
{
    buf[0] = buf[1] = buf[2] = ' ';

    if (val < 10)
    {
        buf[0] = val + '0';
    }
    else if (val < 100)
    {
        buf[0] = val / 10 + '0';
        buf[1] = val % 10 + '0';
    }
    else if (val < 1000)
    {
        buf[0] = val / 100 + '0';
        val %= 100;
        buf[1] = val / 10 + '0';
        buf[2] = val % 10 + '0';
    }
}

void initMazeWallToDisplay(char buf[][20])
{
    for (int row = 0; row < 5; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            buf[row][col] = ' ';
        }
    }

    for (int row = 0; row < 5; ++row)
    {
        buf[row][0]  = '|';
        buf[row][10] = '|';
    }

    for (int col = 1; col < 10; ++col)
    {
        buf[4][col] = '_';
    }
}

const char* toCString(logic::MouseDir dir)
{
    using logic::MouseDir;
    switch (dir)
    {
    case MouseDir::N:
        return "N";
    case MouseDir::W:
        return "W";
    case MouseDir::S:
        return "S";
    case MouseDir::E:
        return "E";
    default:
        return "unknown";
    }
}

}  // namespace

namespace logic
{

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

bool operator!=(const Point& lhs, const Point& rhs)
{
    return not(lhs == rhs);
}

Maze::Maze(utils::AllSignals& signals)
    : mSignals{signals}
    , mStart{15, 15}
    , mFinish{12, 15}  // , mIsCellsConnected{}
    , mMouseInitDir{MouseDir::N}
{
    mCurrentPos = mStart;
    mMouseDir   = mMouseInitDir;
    initFloodfill();
}

void Maze::updateMouseAndMazeState(const controller::Command& cmd, const mycha::DistancesData& distances)
{
    if (cmd.type == controller::CommandType::Forward)
    {
        // update only one cell
        if (cmpEqual(cmd.forward.length, mycha::mechanic::labyrinthCubeSize))
        {
            moveMouseForward();
            updateWalls(distances);
            floodfill(mCurrentPos);
        }
    }
    else if (cmd.type == controller::CommandType::Rotational)
    {
        rotateMouse(cmd.rotational.rotDir);
    }
}

void Maze::updateWallsAtStartup(const mycha::DistancesData& distances)
{
    updateWalls(distances);
    floodfill(mCurrentPos);
}

void Maze::moveMouseForward()
{
    static int cntForward = 0;
    ++cntForward;
    auto x = mCurrentPos.x;
    auto y = mCurrentPos.y;

    if (mMouseDir == MouseDir::N)
    {
        --y;
    }
    else if (mMouseDir == MouseDir::W)
    {
        --x;
    }
    else if (mMouseDir == MouseDir::S)
    {
        ++y;
    }
    else if (mMouseDir == MouseDir::E)
    {
        ++x;
    }

    // should newer happen, but to make sure soft won't crash...
    if (x < 0 || x >= mazeLen || y < 0 || y >= mazeLen)
        return;

    // markConnectionBetweenCells(mCurrentPos, Point{x, y});

    mCurrentPos.x = x;
    mCurrentPos.y = y;

    // mSignals.displayLogValue.emit("x: %f", (float)mCurrentPos.x, 0, false);
    // mSignals.displayLogValue.emit("y: %f", (float)mCurrentPos.y, 1, false);
    // mSignals.displayLogValue.emit("cnt: %f", (float)cntForward, 2, true);
}

void Maze::markConnectionBetweenCells(const Point& p1, const Point& p2)
{
    // const uint32_t cell1Number = p1.y * mazeLen + (p1.x + 1);
    // const uint32_t cell2Number = p2.y * mazeLen + (p2.x + 1);

    // mIsCellsConnected[cell1Number - 1][cell2Number - 1] = true;
    // mIsCellsConnected[cell2Number - 1][cell1Number - 1] = true;
}

void Maze::rotateMouse(controller::RotationDir dir)
{
    using controller::RotationDir;
    if (dir == RotationDir::Left)
    {
        if (mMouseDir == MouseDir::N)
        {
            mMouseDir = MouseDir::W;
        }
        else if (mMouseDir == MouseDir::W)
        {
            mMouseDir = MouseDir::S;
        }
        else if (mMouseDir == MouseDir::S)
        {
            mMouseDir = MouseDir::E;
        }
        else if (mMouseDir == MouseDir::E)
        {
            mMouseDir = MouseDir::N;
        }
    }
    else if (dir == RotationDir::Right)
    {
        if (mMouseDir == MouseDir::N)
        {
            mMouseDir = MouseDir::E;
        }
        else if (mMouseDir == MouseDir::E)
        {
            mMouseDir = MouseDir::S;
        }
        else if (mMouseDir == MouseDir::S)
        {
            mMouseDir = MouseDir::W;
        }
        else if (mMouseDir == MouseDir::W)
        {
            mMouseDir = MouseDir::N;
        }
    }
    else if (dir == RotationDir::TurnBack)
    {
        if (mMouseDir == MouseDir::N)
        {
            mMouseDir = MouseDir::S;
        }
        else if (mMouseDir == MouseDir::S)
        {
            mMouseDir = MouseDir::N;
        }
        else if (mMouseDir == MouseDir::E)
        {
            mMouseDir = MouseDir::W;
        }
        else if (mMouseDir == MouseDir::W)
        {
            mMouseDir = MouseDir::E;
        }
    }
}

void Maze::updateWalls(const mycha::DistancesData& distances)
{
    if (mMouseDir == MouseDir::N)
    {
        if (sensors::isWall(distances.front))
        {
            updateWallN(mCurrentPos);
        }
        if (sensors::isWall(distances.left))
        {
            updateWallW(mCurrentPos);
        }
        if (sensors::isWall(distances.right))
        {
            updateWallE(mCurrentPos);
        }
    }
    else if (mMouseDir == MouseDir::W)
    {
        if (sensors::isWall(distances.front))
        {
            updateWallW(mCurrentPos);
        }
        if (sensors::isWall(distances.left))
        {
            updateWallS(mCurrentPos);
        }
        if (sensors::isWall(distances.right))
        {
            updateWallN(mCurrentPos);
        }
    }
    else if (mMouseDir == MouseDir::S)
    {
        if (sensors::isWall(distances.front))
        {
            updateWallS(mCurrentPos);
        }
        if (sensors::isWall(distances.left))
        {
            updateWallE(mCurrentPos);
        }
        if (sensors::isWall(distances.right))
        {
            updateWallW(mCurrentPos);
        }
    }
    else if (mMouseDir == MouseDir::E)
    {
        if (sensors::isWall(distances.front))
        {
            updateWallE(mCurrentPos);
        }
        if (sensors::isWall(distances.left))
        {
            updateWallN(mCurrentPos);
        }
        if (sensors::isWall(distances.right))
        {
            updateWallS(mCurrentPos);
        }
    }
}

void Maze::updateWallN(const Point& point)
{
    if (isConnectionBetweenCells(point, Point{point.x, point.y - 1}))
        return;

    mMaze[point.y][point.x].isWallN = true;
    if (point.y - 1 >= 0)
    {
        mMaze[point.y - 1][point.x].isWallS = true;
    }
}

void Maze::updateWallW(const Point& point)
{
    if (isConnectionBetweenCells(point, Point{point.x - 1, point.y}))
        return;

    mMaze[point.y][point.x].isWallW = true;
    if (point.x - 1 >= 0)
    {
        mMaze[point.y][point.x - 1].isWallE = true;
    }
}

void Maze::updateWallS(const Point& point)
{
    if (isConnectionBetweenCells(point, Point{point.x, point.y + 1}))
        return;

    mMaze[point.y][point.x].isWallS = true;
    if (point.y + 1 < mazeLen)
    {
        mMaze[point.y + 1][point.x].isWallN = true;
    }
}

void Maze::updateWallE(const Point& point)
{
    if (isConnectionBetweenCells(point, Point{point.x + 1, point.y}))
        return;

    mMaze[point.y][point.x].isWallE = true;
    if (point.x + 1 < mazeLen)
    {
        mMaze[point.y][point.x + 1].isWallW = true;
    }
}

bool Maze::isConnectionBetweenCells(const Point& p1, const Point& p2)
{
    // if (p1.x < 0 || p1.x >= mazeLen || p1.y < 0 || p1.y >= mazeLen)
    //     return false;
    // if (p2.x < 0 || p2.x >= mazeLen || p2.y < 0 || p2.y >= mazeLen)
    //     return false;

    // const uint32_t cell1Number = p1.y * mazeLen + (p1.x + 1);
    // const uint32_t cell2Number = p2.y * mazeLen + (p2.x + 1);

    // return mIsCellsConnected[cell1Number - 1][cell2Number - 1];
    return false;
}

void Maze::drawMazeWeights()
{
    // if (mazeLen != 5)
    //     return;

    char buf[3];
    for (int row = 0; row < mazeLen; ++row)
    {
        char* rowPtr = display::displayMazeBuff[row];
        for (int col = 0; col < mazeLen; ++col)
        {
            valueToCharBuf(mMaze[row][col].value, buf);
            *rowPtr++ = buf[0];
            *rowPtr++ = buf[1];
            *rowPtr++ = buf[2];
            if (col < 15)
            {
                *rowPtr++ = '|';
            }
        }
    }
}

void Maze::drawMazeWalls()
{
    if (mazeLen != 5)
        return;

    static bool isMazeWallInit = false;
    if (not isMazeWallInit)
    {
        initMazeWallToDisplay(bufMazeWalls);
        isMazeWallInit = true;
    }

    auto x = mCurrentPos.x;
    auto y = mCurrentPos.y;

    // north, upper wall is impossible to draw
    if (mMaze[y][x].isWallN)
    {
        if (y > 0)
        {
            bufMazeWalls[y - 1][x * 2 + 1] = '_';
        }
    }
    // west
    if (mMaze[y][x].isWallW)
    {
        bufMazeWalls[y][x * 2] = '|';
    }
    // south
    if (mMaze[y][x].isWallS)
    {
        bufMazeWalls[y][x * 2 + 1] = '_';
    }
    // east
    if (mMaze[y][x].isWallE)
    {
        bufMazeWalls[y][x * 2 + 2] = '|';
    }
}

void Maze::logMouseAndMaze(utils::LoggingSystem& logger)
{
    // 16 rows and 70 columns + null
    static constexpr uint32_t bufSize = 16 * 70 + 1;
    static char bufWeights[bufSize];
    static char bufWalls[bufSize];

    memset(bufWeights, 0, sizeof(bufWeights));
    memset(bufWalls, 0, sizeof(bufWalls));

    // print weights
    memset(display::displayMazeBuff, 0, sizeof(display::displayMazeBuff));
    drawMazeWeights();
    // to have null at the end
    for (int i = 0; i < 16; i++)
        display::displayMazeBuff[i][69] = 0;
    sprintf(bufWeights, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", display::displayMazeBuff[0],
            display::displayMazeBuff[1], display::displayMazeBuff[2], display::displayMazeBuff[3],
            display::displayMazeBuff[4], display::displayMazeBuff[5], display::displayMazeBuff[6],
            display::displayMazeBuff[7], display::displayMazeBuff[8], display::displayMazeBuff[9],
            display::displayMazeBuff[10], display::displayMazeBuff[11], display::displayMazeBuff[12],
            display::displayMazeBuff[13], display::displayMazeBuff[14], display::displayMazeBuff[15]);

    logger.info("X=%d, Y=%d, dir=%s\nWeights:\n%s\n\n", mCurrentPos.x, mCurrentPos.y, toCString(mMouseDir), bufWeights,
                bufWalls);

    // // print walls
    // drawMazeWalls();
    // // to have null at the end
    // for (int i = 0; i < 5; i++)
    //     bufMazeWalls[i][19] = 0;
    // sprintf(bufWalls, "%s\n%s\n%s\n%s\n%s\n", bufMazeWalls[0], bufMazeWalls[1], bufMazeWalls[2], bufMazeWalls[3],
    //         bufMazeWalls[4]);

    // logger.info("X=%d, Y=%d, dir=%s\nWeights:\n%s\nWalls:\n%s\n\n", mCurrentPos.x, mCurrentPos.y,
    // toCString(mMouseDir),
    //             bufWeights, bufWalls);
}

bool Maze::isMouseInFinishPoint()
{
    return mCurrentPos == mFinish;
}

bool Maze::isMouseInStartingPoint()
{
    return mCurrentPos == mStart;
}

controller::Command Maze::generateCommandInSearchRun() const
{
    Point nextPoint = findCellWithMinValueFromNeighbour();
    return getCommandFromCurrentPointToNext(nextPoint);
}

void Maze::generateCommandsToBackToStart(controller::CommandQueue& queue)
{
    using namespace controller;

    const Point originalCurrentPos  = mCurrentPos;
    const MouseDir originalMouseDir = mMouseDir;

    mCurrentPos = mStart;
    mMouseDir   = mMouseInitDir;

    // simulate run from start to finish

    mSignals.setLed2.emit(true);
    while (not isMouseInFinishPoint())
    {
        auto cmd = Maze::generateCommandInSearchRun();
        if (cmd.type == CommandType::Forward)
            moveMouseForward();
        else if (cmd.type == CommandType::Rotational)
            rotateMouse(cmd.rotational.rotDir);
        cmdStack.push(cmd);
        if (cmdStack.isFull())
            mSignals.setLed3.emit(true);
    }
    mSignals.setLed4.emit(true);

    // insert first command, turn back from finish
    Command cmd{};
    cmd.type       = CommandType::Rotational;
    cmd.rotational = RotationalCommand{RotationDir::TurnBack};
    queue.addCommand(cmd);

    // reverse commands to make it from finish to start
    // optimize it for speed on straight road
    while (not cmdStack.isEmpty())
    {
        auto popedCmd = cmdStack.pop();
        if (popedCmd.type == CommandType::Rotational)
        {
            Command cmd{};
            if (popedCmd.rotational.rotDir == RotationDir::Left)
            {
                cmd.type       = CommandType::Rotational;
                cmd.rotational = RotationalCommand{RotationDir::Right};
            }
            else if (popedCmd.rotational.rotDir == RotationDir::Right)
            {
                cmd.type       = CommandType::Rotational;
                cmd.rotational = RotationalCommand{RotationDir::Left};
            }
            queue.addCommand(cmd);
        }
        else if (popedCmd.type == CommandType::Forward)
        {
            uint32_t nrOfForwardCommands = 1;
            while (not cmdStack.isEmpty() and cmdStack.peek().type == CommandType::Forward)
            {
                ++nrOfForwardCommands;
                cmdStack.pop();
            }
            Command cmd{};
            cmd.type    = CommandType::Forward;
            cmd.forward = ForwardCommand{nrOfForwardCommands * mycha::mechanic::labyrinthCubeSize};
            queue.addCommand(cmd);
        }
    }

    // add command to turn around to be ready to fast run
    cmd.type       = CommandType::Rotational;
    cmd.rotational = RotationalCommand{RotationDir::TurnBack};
    queue.addCommand(cmd);

    mCurrentPos = originalCurrentPos;
    mMouseDir   = originalMouseDir;
}

void Maze::initFloodfill()
{
    for (int x = 0; x < mazeLen; x++)
    {
        for (int y = 0; y < mazeLen; y++)
            mMaze[x][y].wasVisited = false;
    }

    mMaze[mFinish.y][mFinish.x].wasVisited = true;
    mMaze[mFinish.y][mFinish.x].value      = 0;
    pushToStackAdjacentCells(mFinish, stack);

    while (not stack.isEmpty())
    {
        Point p        = stack.pop();
        uint8_t minVal = minValueFromNeighbours(p);
        if (mMaze[p.y][p.x].value + 1 != minVal)
        {
            mMaze[p.y][p.x].value      = minVal + 1;
            mMaze[p.y][p.x].wasVisited = true;
            pushToStackAdjacentCells(p, stack);
        }
    }
}

void Maze::floodfill(Point floodP)
{
    for (int x = 0; x < mazeLen; x++)
    {
        for (int y = 0; y < mazeLen; y++)
            mMaze[x][y].wasVisited = false;
    }

    if (mCurrentPos == mFinish)
        return;

    mMaze[mFinish.y][mFinish.x].wasVisited = true;

    pushToStackAdjacentCells(floodP, stack);
    stack.push(floodP);

    while (not stack.isEmpty())
    {
        Point p        = stack.pop();
        uint8_t minVal = minValueFromNeighbours(p);
        if (mMaze[p.y][p.x].value != minVal + 1)
        {
            mMaze[p.y][p.x].value      = minVal + 1;
            mMaze[p.y][p.x].wasVisited = true;
            pushToStackAdjacentCells(p, stack);
        }
    }
}

uint8_t Maze::minValueFromNeighbours(Point p)
{
    uint8_t minVal = 255;
    uint8_t tmp    = 255;

    Cell& cell = mMaze[p.y][p.x];
    // north
    if (!cell.isWallN and p.y - 1 >= 0)
    {
        minVal = mMaze[p.y - 1][p.x].value;
    }
    // west
    if (!cell.isWallW and p.x - 1 >= 0)
    {
        tmp    = mMaze[p.y][p.x - 1].value;
        minVal = tmp < minVal ? tmp : minVal;
    }
    // south
    if (!cell.isWallS and p.y + 1 < mazeLen)
    {
        tmp    = mMaze[p.y + 1][p.x].value;
        minVal = tmp < minVal ? tmp : minVal;
    }
    // east
    if (!cell.isWallE and p.x + 1 < mazeLen)
    {
        tmp    = mMaze[p.y][p.x + 1].value;
        minVal = tmp < minVal ? tmp : minVal;
    }
    return minVal;
}

void Maze::pushToStackAdjacentCells(const Point& p, StackType& stack)
{
    Cell& cell = mMaze[p.y][p.x];
    // north
    if (!cell.isWallN and p.y - 1 >= 0)
    {
        if (!mMaze[p.y - 1][p.x].wasVisited)
        {
            stack.push(Point{p.x, p.y - (int8_t)1});
        }
    }
    // west
    if (!cell.isWallW and p.x - 1 >= 0)
    {
        if (!mMaze[p.y][p.x - 1].wasVisited)
        {
            stack.push(Point{p.x - (int8_t)1, p.y});
        }
    }
    // south
    if (!cell.isWallS and p.y + 1 < mazeLen)
    {
        if (!mMaze[p.y + 1][p.x].wasVisited)
        {
            stack.push(Point{p.x, p.y + (int8_t)1});
        }
    }
    // east
    if (!cell.isWallE and p.x + 1 < mazeLen)
    {
        if (!mMaze[p.y][p.x + 1].wasVisited)
        {
            stack.push(Point{p.x + (int8_t)1, p.y});
        }
    }
}

Point Maze::findCellWithMinValueFromNeighbour() const
{
    uint8_t minVal = 255;
    uint8_t tmp    = 255;
    Point minPoint{};

    const Point& p   = mCurrentPos;
    const Cell& cell = mMaze[p.y][p.x];
    // north
    if (!cell.isWallN and p.y - 1 >= 0)
    {
        minVal     = mMaze[p.y - 1][p.x].value;
        minPoint.x = p.x;
        minPoint.y = p.y - 1;
    }
    // west
    if (!cell.isWallW and p.x - 1 >= 0)
    {
        tmp = mMaze[p.y][p.x - 1].value;
        if (tmp < minVal)
        {
            minVal     = tmp;
            minPoint.x = p.x - 1;
            minPoint.y = p.y;
        }
    }
    // south
    if (!cell.isWallS and p.y + 1 < mazeLen)
    {
        tmp = mMaze[p.y + 1][p.x].value;
        if (tmp < minVal)
        {
            minVal     = tmp;
            minPoint.x = p.x;
            minPoint.y = p.y + 1;
        }
    }
    // east
    if (!cell.isWallE and p.x + 1 < mazeLen)
    {
        tmp = mMaze[p.y][p.x + 1].value;
        if (tmp < minVal)
        {
            minVal     = tmp;
            minPoint.x = p.x + 1;
            minPoint.y = p.y;
        }
    }

    return minPoint;
}

controller::Command Maze::getCommandFromCurrentPointToNext(Point next) const
{
    static constexpr double wallLen = 0.18;

    using namespace controller;

    const Point& p = mCurrentPos;
    MouseDir dirOfNextPointFromCurrent{};

    if (next.x > p.x)
        dirOfNextPointFromCurrent = MouseDir::E;
    else if (next.x < p.x)
        dirOfNextPointFromCurrent = MouseDir::W;
    else if (next.y < p.y)
        dirOfNextPointFromCurrent = MouseDir::N;
    else
        dirOfNextPointFromCurrent = MouseDir::S;

    // we can go forward
    if (dirOfNextPointFromCurrent == mMouseDir)
    {
        Command cmd{};
        cmd.type           = CommandType::Forward;
        cmd.forward.length = wallLen;
        return cmd;
    }

    // we must first rotate
    Command cmd;
    cmd.type = CommandType::Rotational;
    if (mMouseDir == MouseDir::N)
    {
        if (dirOfNextPointFromCurrent == MouseDir::W)
            cmd.rotational = RotationalCommand{RotationDir::Left};
        else if (dirOfNextPointFromCurrent == MouseDir::S)
            cmd.rotational = RotationalCommand{RotationDir::TurnBack};
        else if (dirOfNextPointFromCurrent == MouseDir::E)
            cmd.rotational = RotationalCommand{RotationDir::Right};
    }
    else if (mMouseDir == MouseDir::W)
    {
        if (dirOfNextPointFromCurrent == MouseDir::N)
            cmd.rotational = RotationalCommand{RotationDir::Right};
        else if (dirOfNextPointFromCurrent == MouseDir::S)
            cmd.rotational = RotationalCommand{RotationDir::Left};
        else if (dirOfNextPointFromCurrent == MouseDir::E)
            cmd.rotational = RotationalCommand{RotationDir::TurnBack};
    }
    else if (mMouseDir == MouseDir::S)
    {
        if (dirOfNextPointFromCurrent == MouseDir::N)
            cmd.rotational = RotationalCommand{RotationDir::TurnBack};
        else if (dirOfNextPointFromCurrent == MouseDir::E)
            cmd.rotational = RotationalCommand{RotationDir::Left};
        else if (dirOfNextPointFromCurrent == MouseDir::W)
            cmd.rotational = RotationalCommand{RotationDir::Right};
    }
    // MouseDir::E
    else
    {
        if (dirOfNextPointFromCurrent == MouseDir::N)
            cmd.rotational = RotationalCommand{RotationDir::Left};
        else if (dirOfNextPointFromCurrent == MouseDir::S)
            cmd.rotational = RotationalCommand{RotationDir::Right};
        else if (dirOfNextPointFromCurrent == MouseDir::W)
            cmd.rotational = RotationalCommand{RotationDir::TurnBack};
    }

    return cmd;
}

}  // namespace logic