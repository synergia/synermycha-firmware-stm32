#pragma once

#include "Mycha.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

namespace mycha
{

class EventHandler : public utils::Observer
{
  public:
    explicit EventHandler(utils::AllSignals& signals);
    void HandleEvents();

  private:
    utils::AllSignals& mSignals;
};

}  // namespace mycha