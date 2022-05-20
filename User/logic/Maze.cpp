#include "Maze.hh"
#include "display/Display.hh"
#include "mycha/Mycha.hh"
#include "sensors/DistanceTof.hh"
#include <cstring>

namespace
{

logic::StackType stack;

bool cmpEqual(double v1, double v2)
{
    return v1 - v2 < 1e-5;
}

// assume that char buf is always 4 size. max value is 255 + \0;
void valueToCharBuf(uint8_t val, char* buf)
{
    buf[0] = buf[1] = buf[2] = buf[3] = 0;

    if (val < 10)
    {
        buf[0] = val + '0';
        buf[1] = ' ';
        buf[2] = ' ';
    }
    else if (val < 100)
    {
        buf[0] = val / 10 + '0';
        buf[1] = val % 10 + '0';
        buf[2] = ' ';
    }
    else if (val < 1000)
    {
        buf[0] = val / 100 + '0';
        val %= 100;
        buf[1] = val / 10 + '0';
        buf[2] = val % 10 + '0';
    }
}

}  // namespace

namespace logic
{

Maze::Maze()
    : mStart{4, 4}
    , mFinish{2, 4}
    , mMouseDir{MouseDir::N}
{
    mCurrentPos = mStart;
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
}

void Maze::moveMouseForward()
{
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

    mCurrentPos.x = x;
    mCurrentPos.y = y;
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
    mMaze[point.y][point.x].isWallN = true;
    if (point.y - 1 >= 0)
    {
        mMaze[point.y - 1][point.x].isWallS = true;
    }
}

void Maze::updateWallW(const Point& point)
{
    mMaze[point.y][point.x].isWallW = true;
    if (point.x - 1 >= 0)
    {
        mMaze[point.y][point.x - 1].isWallE = true;
    }
}

void Maze::updateWallS(const Point& point)
{
    mMaze[point.y][point.x].isWallS = true;
    if (point.y + 1 < mazeLen)
    {
        mMaze[point.y + 1][point.x].isWallN = true;
    }
}

void Maze::updateWallE(const Point& point)
{
    mMaze[point.y][point.x].isWallE = true;
    if (point.x + 1 < mazeLen)
    {
        mMaze[point.y][point.x + 1].isWallW = true;
    }
}

void Maze::drawMaze()
{
    char buf[4];
    for (int row = 0; row < 5; ++row)
    {
        char* rowPtr = display::displayMazeBuff[row];
        for (int col = 0; col < 5; ++col)
        {
            valueToCharBuf(mMaze[row][col].value, buf);
            strcat(rowPtr, buf);
            rowPtr += 3;
            if (col < 4)
            {
                strcat(rowPtr, "|");
                ++rowPtr;
            }
        }
    }
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

}  // namespace logic