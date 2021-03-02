#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

namespace utils
{

// @TODO do we need this function?
static void FatalError(const char* message)
{
    while (true)
        ;
}

}  // namespace utils