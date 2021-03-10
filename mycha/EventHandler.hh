#pragma once

#include "Mycha.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

class EventHandler : public utils::Observer
{
  public:
    EventHandler(utils::AllSignals& signals);
    void HandleEvents();

  private:
    utils::AllSignals& mSignals;
};