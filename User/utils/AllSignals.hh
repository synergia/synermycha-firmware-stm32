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

    signal<void(void)> interruptDistance;
    signal<void(void)> tim14Elapsed;

    // special signal, called directly from interupt routine
    signal<void(void)> interruptController;
};

}  // namespace utils