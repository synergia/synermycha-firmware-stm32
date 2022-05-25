#pragma once

#include "Signal.hh"
#include "mycha/MouseData.hh"

namespace utils
{

struct AllSignals
{
    signal<void(void)> buttonUp;
    signal<void(void)> buttonDown;
    signal<void(void)> buttonEnter;

    signal<void(void)> displayBuffor;
    // used in controll loop to not delay it. Only set displayBuffor, call displayBuffReadyPararell
    // and then in EventHandler displayBuffPararell is always called to check if new draw is needed or not
    signal<void(void)> displayBuffPararell;
    signal<void(void)> displayBuffReadyPararell;
    // show informs if driver should load bytes to memory,
    // so if more logs are needed, only last log should have show=true, to avoid latency
    signal<void(const char* text, double value, uint8_t line, bool show)> displayLogValue;
    signal<void(int line, const char* text)> displayTextLine;
    signal<void(void)> showMaze;
    signal<void(void)> mazeReadyToShow;

    signal<void(void)> interruptDistance;
    signal<void(void)> tim14Elapsed;

    // special signal, called directly from interupt routine
    signal<void(void)> interruptController;

    signal<void(const mycha::DrivingData&)> setDrivingData;
    signal<void(const mycha::DistancesData&)> setDistancesData;

    signal<void(mycha::MotorsSettings&)> getMotorSettings;

    signal<void(double&)> getGyroZ;
};

}  // namespace utils