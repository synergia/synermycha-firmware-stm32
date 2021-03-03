#pragma once

#include "MenuPage.hh"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"
#include <array>

namespace menu
{

constexpr int maxNrOfItemMenus{10};

enum class MenuState
{
    Normal,   // switching in options
    Changing  // giving controll to option
};

class Menu : public utils::Observer
{
  public:
    Menu(utils::AllSignals& sig);

    void setDefaultMenuPage(MenuPage* page);
    void onButtonDownPressed();
    void onButtonUpPressed();
    void onEnterPressed();

  private:
    utils::AllSignals& mSignals;
    MenuPage* mActualPage;
    MenuState mState;
    bool isFirstRun = true;
};

}  // namespace menu