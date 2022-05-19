#include "Maze.hh"
#include "mycha/Mycha.hh"
#include "sensors/DistanceTof.hh"

namespace
{

bool cmpEqual(double v1, double v2)
{
    return v1 - v2 < 1e-5;
}

}  // namespace

namespace logic
{

Maze::Maze()
    : mStart{5, 1}
    , mFinish{3, 3}
    , mMouseDir{MouseDir::N}
{
    mCurrentPos = mStart;
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
        ++y;
    }
    else if (mMouseDir == MouseDir::W)
    {
        --x;
    }
    else if (mMouseDir == MouseDir::S)
    {
        --y;
    }
    else if (mMouseDir == MouseDir::E)
    {
        ++x;
    }

    // should newer happen, but to make sure soft won't crash...
    if (x < 1 || x > mazeLen || y < 1 || y > mazeLen)
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
    mMaze[point.x][point.y].isWallN = true;
    if (point.y + 1 <= mazeLen)
    {
        mMaze[point.x][point.y + 1].isWallS = true;
    }
}

void Maze::updateWallW(const Point& point)
{
    mMaze[point.x][point.y].isWallW = true;
    if (point.x - 1 >= 0)
    {
        mMaze[point.x - 1][point.y].isWallE = true;
    }
}

void Maze::updateWallS(const Point& point)
{
    mMaze[point.x][point.y].isWallS = true;
    if (point.y - 1 >= 0)
    {
        mMaze[point.x][point.y - 1].isWallN = true;
    }
}

void Maze::updateWallE(const Point& point)
{
    mMaze[point.x][point.y].isWallE = true;
    if (point.x + 1 <= mazeLen)
    {
        mMaze[point.x + 1][point.y].isWallW = true;
    }
}

}  // namespace logic