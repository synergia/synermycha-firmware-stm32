#pragma once

#include "Signal.hh"

namespace utils
{

struct AllSignals
{
    signal<void(void)> buttonUp;
    signal<void(void)> buttonDown;
    signal<void(void)> buttonEnter;

    signal<void(void)> displayBuffor;
};

}  // namespace utils